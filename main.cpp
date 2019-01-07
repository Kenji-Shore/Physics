#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "loadShader.cpp"
#include "loadObject.cpp"
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <vector>
#include "Object.h"

using namespace std;
using namespace glm;

int main() {
    bool quit = false;
    SDL_Window* window;
    SDL_GLContext glContext;
    SDL_Event sdlEvent;

    float i = 0;
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    int windowX = 800;
    int windowY = 600;

    window = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowX, windowY, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (window == NULL) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    glContext = SDL_GL_CreateContext(window);
    if (glContext == NULL) {
        std::cout << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    vector <glm::vec3> vertices;
    vector <glm::vec2> uvs;
    vector <glm::vec3> normals;

    bool res = LoadObjects("../cube.obj", vertices, uvs, normals);

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

    GLuint programID = LoadShaders("../Vertex.vert", "../Fragment.frag");

    int x, y, bpp;
    unsigned char* data = stbi_load("../tex.jpeg", &x, &y, 0, 3);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    stbi_image_free(data);

    vec3 colors[] = {
        vec3 (0, 0, 0), //0, black
        vec3 (1, 0, 0), //1, red
        vec3 (0, 1, 0), //2, green
        vec3 (0, 0, 1), //3, blue
        vec3 (1, 0.55, 0), //4, orange
        vec3 (1, 1, 0), //5, yellow
        vec3 (1, 1, 1), //6, white
    };

    struct cubeData {
        vec3 position;
        int faces[3];
        mat4 rotation;
    };

    //negative x: 4 orange
    //positive x: 1 red
    //negative z: 2 green
    //positive z: 3 blue
    //negative y: 5 yellow
    //positive y: 6 white

    //currently showing red, white, blue

    cubeData arrangement[26] = {
        {.position = vec3 (-2,-2,-2), .faces = {5, 4, 2}, .rotation = rotate((float) -M_PI / 2, vec3(0, 1, 0)) * rotate((float) M_PI / 2, vec3(1, 0, 0))},
        {.position = vec3 (-2,-2, 0), .faces = {5, 4, 0}, .rotation = rotate((float) -M_PI / 2, vec3(0, 1, 0)) * rotate((float) M_PI / 2, vec3(1, 0, 0))},
        {.position = vec3 (-2,-2, 2), .faces = {5, 3, 4}, .rotation = rotate((float) M_PI / 2, vec3(1, 0, 0))},
        {.position = vec3 ( 0,-2,-2), .faces = {5, 0, 2}, .rotation = rotate((float) -M_PI / 2, vec3(0, 1, 0)) * rotate((float) M_PI / 2, vec3(1, 0, 0))},
        {.position = vec3 ( 0,-2, 0), .faces = {5, 0, 0}, .rotation = rotate((float) M_PI / 2, vec3(1, 0, 0))}, //yellow
        {.position = vec3 ( 0,-2, 2), .faces = {5, 0, 3}, .rotation = rotate((float) M_PI / 2, vec3(0, 1, 0)) * rotate((float) M_PI / 2, vec3(1, 0, 0))},
        {.position = vec3 ( 2,-2,-2), .faces = {5, 2, 1}, .rotation = rotate((float) -M_PI, vec3(0, 1, 0)) * rotate((float) M_PI / 2, vec3(1, 0, 0))},
        {.position = vec3 ( 2,-2, 0), .faces = {5, 1, 0}, .rotation = rotate((float) M_PI / 2, vec3(0, 1, 0)) * rotate((float) M_PI / 2, vec3(1, 0, 0))},
        {.position = vec3 ( 2,-2, 2), .faces = {5, 1, 3}, .rotation = rotate((float) M_PI / 2, vec3(0, 1, 0)) * rotate((float) M_PI / 2, vec3(1, 0, 0))},

        {.position = vec3 (-2, 0,-2), .faces = {0, 4, 2}, .rotation = rotate((float) M_PI / 2, vec3(1, 0, 0)) * rotate((float) M_PI / 2, vec3(0, 0, 1))},
        {.position = vec3 (-2, 0, 0), .faces = {0, 4, 0}, .rotation = rotate((float) M_PI / 2, vec3(0, 0, 1))}, //orange
        {.position = vec3 (-2, 0, 2), .faces = {0, 4, 3}, .rotation = rotate((float) -M_PI / 2, vec3(1, 0, 0)) * rotate((float) M_PI / 2, vec3(0, 0, 1))},
        {.position = vec3 ( 0, 0,-2), .faces = {0, 0, 2}, .rotation = rotate((float) -M_PI / 2, vec3(0, 1, 0))}, //green
        //Center 0, 0, 0
        {.position = vec3 ( 0, 0, 2), .faces = {0, 0, 3}, .rotation = rotate((float) M_PI / 2, vec3(0, 1, 0))}, //blue
        {.position = vec3 ( 2, 0,-2), .faces = {0, 1, 2}, .rotation = rotate((float) -M_PI / 2, vec3(1, 0, 0)) * rotate((float) -M_PI / 2, vec3(0, 0, 1))},
        {.position = vec3 ( 2, 0, 0), .faces = {0, 1, 0}, .rotation = rotate((float) -M_PI / 2, vec3(0, 0, 1))}, //red
        {.position = vec3 ( 2, 0, 2), .faces = {0, 1, 3}, .rotation = rotate((float) M_PI / 2, vec3(1, 0, 0)) * rotate((float) -M_PI / 2, vec3(0, 0, 1))},

        {.position = vec3 (-2, 2,-2), .faces = {6, 2, 4}, .rotation = rotate((float) -M_PI / 2, vec3(1, 0, 0))},
        {.position = vec3 (-2, 2, 0), .faces = {6, 4, 0}, .rotation = rotate((float) M_PI / 2, vec3(0, 1, 0)) * rotate((float) -M_PI / 2, vec3(1, 0, 0))},
        {.position = vec3 (-2, 2, 2), .faces = {6, 4, 3}, .rotation = rotate((float) M_PI / 2, vec3(0, 1, 0)) * rotate((float) -M_PI / 2, vec3(1, 0, 0))},
        {.position = vec3 ( 0, 2,-2), .faces = {6, 0, 2}, .rotation = rotate((float) -M_PI / 2, vec3(0, 1, 0)) * rotate((float) -M_PI / 2, vec3(1, 0, 0))},
        {.position = vec3 ( 0, 2, 0), .faces = {6, 0, 0}, .rotation = rotate((float) -M_PI / 2, vec3(1, 0, 0))}, //white
        {.position = vec3 ( 0, 2, 2), .faces = {6, 0, 3}, .rotation = rotate((float) M_PI / 2, vec3(0, 1, 0)) * rotate((float) -M_PI / 2, vec3(1, 0, 0))},
        {.position = vec3 ( 2, 2,-2), .faces = {6, 1, 2}, .rotation = rotate((float) -M_PI / 2, vec3(0, 1, 0)) * rotate((float) -M_PI / 2, vec3(1, 0, 0))},
        {.position = vec3 ( 2, 2, 0), .faces = {6, 1, 0}, .rotation = rotate((float) -M_PI / 2, vec3(0, 1, 0)) * rotate((float) -M_PI / 2, vec3(1, 0, 0))},
        {.position = vec3 ( 2, 2, 2), .faces = {6, 3, 1}, .rotation = rotate((float) M_PI, vec3(0, 1, 0)) * rotate((float) -M_PI / 2, vec3(1, 0, 0))},
    };

    vector <Object> cubes = {};

    //rotate((float) M_PI, vec3(0, 1, 0))
    vec3 colorStore[26][3] {};

    for (unsigned int i = 0; i < 26; i++) {
        colorStore[i][0] = colors[arrangement[i].faces[0]];
        colorStore[i][1] = colors[arrangement[i].faces[1]];
        colorStore[i][2] = colors[arrangement[i].faces[2]];

        Object cube (arrangement[i].position, vec3(1, 1, 1), arrangement[i].rotation, colorStore[i]);
        cubes.push_back(cube);
    }

    float horizontalAngle = M_PI * 0.25f;
    float verticalAngle = M_PI * 0.75f;

    float zoom = 15.0f;
    float mouseSpeed = 0.005f;

    int lastTime = SDL_GetTicks();

    while (!quit) {
        float deltaTime = (float) (SDL_GetTicks() - lastTime) / 1000.0f;
        lastTime = SDL_GetTicks();

        int x, y;
        SDL_GetMouseState(&x, &y);
        double xpos = (double) x;
        double ypos = (double) y;

        SDL_WarpMouseInWindow(window, windowX / 2, windowY / 2);

        horizontalAngle += mouseSpeed * float(windowX / 2 - xpos);
        verticalAngle = fmin(fmax(verticalAngle - mouseSpeed * float(windowY / 2 - ypos), -M_PI * 0.4f), M_PI * 0.4f);

        vec3 direction(
            cos(verticalAngle) * sin(horizontalAngle),
            sin(verticalAngle),
            cos(verticalAngle) * cos(horizontalAngle)
        );

        vec3 right (sin(horizontalAngle - 3.14f/2.0f), 0, cos(horizontalAngle - 3.14f/2.0f));

        vec3 up = cross(right, direction);

        while (SDL_PollEvent(&sdlEvent) != 0) {
            if (sdlEvent.type == SDL_QUIT) {
                quit = true;
            } else if (sdlEvent.type == SDL_MOUSEWHEEL) {
                zoom = fmin(fmax(zoom - sdlEvent.wheel.y, 8.0f), 30.0f);
            }
        }

        glClearColor(0.8, 0.8, 0.8, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(programID);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        //Camera projection
        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) windowX / windowY, 0.1f, 100.0f);

        //Camera matrix
        glm::mat4 View = lookAt(zoom * vec3(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle)), vec3 (0, 0, 0), vec3(0, 1, 0));

        //glm::mat4 scale = glm::scale(glm::vec3(1, 1, 1));
        //glm::mat4 rotate = glm::rotate(i, glm::vec3(1.0f, 0.0f, 0.0f));
        //glm::mat4 translate = glm::translate(glm::vec3(0, 0, 0));

        //glm::mat4 Model = translate * rotate * scale;

        //glm::mat4 mvp = Projection * View * Model;

        for (unsigned int i = 0; i < cubes.size(); i++) {
            cubes[i].Draw(programID, Projection, View, vertices.size());
        };

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        SDL_GL_SwapWindow(window);
        i = i + 0.002f;
    }

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();

    return 0;
}