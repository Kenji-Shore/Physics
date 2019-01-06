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

using namespace std;
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

    while (!quit) {
        while (SDL_PollEvent(&sdlEvent) != 0) {
            if (sdlEvent.type == SDL_QUIT) {
                quit = true;
            }
        }

        glClearColor(1, 1, 0, 1);
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
        glm::mat4 View = glm::lookAt(glm::vec3(10 * cos(i), 3, 10 * sin(i)), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

        glm::mat4 scale = glm::scale(glm::vec3(1, 1, 1));
        glm::mat4 rotate = glm::rotate(i, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 translate = glm::translate(glm::vec3(0, 0, 0));

        glm::mat4 Model = translate * rotate * scale;

        glm::mat4 mvp = Projection * View * Model;

        GLuint MatrixID = glGetUniformLocation(programID, "MVP");
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

        glm::vec3 colors[] = {
                glm::vec3(0, 0.1, 0.6),
                glm::vec3(0, 0.5, 1),
                glm::vec3(0.3, 0.1, 0.3),
        };

        GLuint ColorsID = glGetUniformLocation(programID, "Colors");
        glUniform3fv(ColorsID, 3, &colors[0][0]);

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
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