#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "shader.h"
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

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

    window = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
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

    static const GLfloat g_vertex_buffer_data[] = {
            -1.0f,-1.0f,-1.0f, // triangle 1 : begin
            -1.0f,-1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f, // triangle 1 : end
            1.0f, 1.0f,-1.0f, // triangle 2 : begin
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f, // triangle 2 : end
            1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f,-1.0f, 1.0f
    };

    static GLfloat g_color_buffer_data[] = {
            0.583f,  0.771f,  0.014f,
            0.609f,  0.115f,  0.436f,
            0.327f,  0.483f,  0.844f,
            0.822f,  0.569f,  0.201f,
            0.435f,  0.602f,  0.223f,
            0.310f,  0.747f,  0.185f,
            0.597f,  0.770f,  0.761f,
            0.559f,  0.436f,  0.730f,
            0.359f,  0.583f,  0.152f,
            0.483f,  0.596f,  0.789f,
            0.559f,  0.861f,  0.639f,
            0.195f,  0.548f,  0.859f,
            0.014f,  0.184f,  0.576f,
            0.771f,  0.328f,  0.970f,
            0.406f,  0.615f,  0.116f,
            0.676f,  0.977f,  0.133f,
            0.971f,  0.572f,  0.833f,
            0.140f,  0.616f,  0.489f,
            0.997f,  0.513f,  0.064f,
            0.945f,  0.719f,  0.592f,
            0.543f,  0.021f,  0.978f,
            0.279f,  0.317f,  0.505f,
            0.167f,  0.620f,  0.077f,
            0.347f,  0.857f,  0.137f,
            0.055f,  0.953f,  0.042f,
            0.714f,  0.505f,  0.345f,
            0.783f,  0.290f,  0.734f,
            0.722f,  0.645f,  0.174f,
            0.302f,  0.455f,  0.848f,
            0.225f,  0.587f,  0.040f,
            0.517f,  0.713f,  0.338f,
            0.053f,  0.959f,  0.120f,
            0.393f,  0.621f,  0.362f,
            0.673f,  0.211f,  0.457f,
            0.820f,  0.883f,  0.371f,
            0.982f,  0.099f,  0.879f
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    GLuint programID = LoadShaders("../Vertex.vert", "../Fragment.frag");

    while (!quit) {
        while (SDL_PollEvent(&sdlEvent) != 0) {
            if (sdlEvent.type == SDL_QUIT) {
                quit = true;
            }
        }

        glClearColor(0, 0, 0.4f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);

        for (int i = 0; i < 12*3; i++) {
            g_color_buffer_data[3*i] = (rand()%100) / 100.0f;
            g_color_buffer_data[3*i + 1] = (rand()%100) / 100.0f;
            g_color_buffer_data[3*i + 2] = (rand()%100) / 100.0f;
        }

        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        //Camera projection
        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 1000.0f/600.0f, 0.1f, 100.0f);

        //Camera matrix
        glm::mat4 View = glm::lookAt(glm::vec3(4, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

        glm::mat4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
        glm::mat4 rotate = glm::rotate(i, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 translate = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));

        glm::mat4 Model = translate * rotate * scale;

        glm::mat4 mvp = Projection * View * Model;

        GLuint MatrixID = glGetUniformLocation(programID, "MVP");
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

        glDrawArrays(GL_TRIANGLES, 0, 12*3);
        glDisableVertexAttribArray(0);
        SDL_GL_SwapWindow(window);
        i = i + 0.001f;
    }

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();

    return 0;
}