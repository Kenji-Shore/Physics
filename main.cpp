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
#include <map>
#include "Object.h"

using namespace std;
using namespace glm;

vec3 colors[] = {
    vec3 (0, 0, 0), //0, black
    vec3 (1, 0, 0), //1, red
    vec3 (0, 0, 1), //2, blue
    vec3 (0, 1, 0), //3, green
    vec3 (1, 0.55, 0), //4, orange
    vec3 (1, 1, 0), //5, yellow
    vec3 (1, 1, 1), //6, white
};


vec3 faceOrder[6][9] = {
    {
        vec3 (2,  2, -2),
        vec3 (2,  0, -2),
        vec3 (2, -2, -2),
        vec3 (2, -2,  0),
        vec3 (2, -2,  2),
        vec3 (2,  0,  2),
        vec3 (2,  2,  2),
        vec3 (2,  2,  0),
        vec3 (2,  0,  0),
    },
    {
        vec3 (-2,  2, 2),
        vec3 (-2,  0, 2),
        vec3 (-2, -2, 2),
        vec3 (-2, -2,  0),
        vec3 (-2, -2, -2),
        vec3 (-2,  0, -2),
        vec3 (-2,  2, -2),
        vec3 (-2,  2,  0),
        vec3 (-2,  0,  0),
    },
    {
        vec3 (2,  2, 2),
        vec3 (2,  0, 2),
        vec3 (2, -2, 2),
        vec3 (0, -2,  2),
        vec3 (-2, -2,  2),
        vec3 (-2,  0,  2),
        vec3 (-2,  2,  2),
        vec3 (0,  2,  2),
        vec3 (0,  0,  2),
    },
    {
        vec3 (-2,  2, -2),
        vec3 (-2,  0, -2),
        vec3 (-2, -2, -2),
        vec3 (0, -2,  -2),
        vec3 (2, -2,  -2),
        vec3 (2,  0,  -2),
        vec3 (2,  2,  -2),
        vec3 (0,  2,  -2),
        vec3 (0,  0,  -2),
    },
    {
        vec3 (0,   2, -2),
        vec3 (2,   2, -2),
        vec3 (2,   2,  0),
        vec3 (2,   2,  2),
        vec3 (0,   2,  2),
        vec3 (-2,  2,  2),
        vec3 (-2,  2,  0),
        vec3 (-2,  2, -2),
        vec3 (0,   2,  0),
    },
    {
        vec3 (0,   -2, -2),
        vec3 (2,   -2, -2),
        vec3 (2,   -2,  0),
        vec3 (2,   -2,  2),
        vec3 (0,   -2,  2),
        vec3 (-2,  -2,  2),
        vec3 (-2,  -2,  0),
        vec3 (-2,  -2, -2),
        vec3 (0,   -2,  0),
    },
};

struct cubeData {
    vec3 position;
    int faces[3];
    mat4 rotation;
};

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

vec3 colorStore[26][3] {};
vector <Object> cubes = {};

struct cubeRotation {
    Object* cube;
    vec3 initialVector;
    mat4 initialRot;
};

struct rotation {
    vector <cubeRotation> cubes;
    vec3 side;
    int direction;
    float angle;
    bool rotating = false;
    float speed = 20.0f;
};

rotation currentRotation;
bool solving = false;
int scrambleCount = 0;

struct moves {
    vec3 side;
    int direction;
};

//positive x: 1 red
//negative x: 4 orange
//positive z: 3 green
//negative z: 2 blue
//positive y: 6 white
//negative y: 5 yellow

vec3 sideOrder[6] = {
    vec3(1, 0, 0),
    vec3(-1, 0, 0),
    vec3(0, 0, 1),
    vec3(0, 0, -1),
    vec3(0, 1, 0),
    vec3(0, -1, 0),
};

string topbottom[9] {
    "b",
    "",
    "r",
    "",
    "g",
    "",
    "o",
    "",
    "",
};

string dictionary[7] {
    "",
    "r",
    "b",
    "g",
    "o",
    "y",
    "w",
};

string shortcuts[6] {
    "r",
    "o",
    "g",
    "b",
    "w",
    "y",
};

string bias[4] {
    "b",
    "o",
    "g",
    "r",
};

void turn (vec3 side, int direction) {
    currentRotation.cubes.clear();
    currentRotation.direction = direction;
    currentRotation.angle = 0.0f;
    currentRotation.side = side;

    for (unsigned int i = 0; i < cubes.size(); i++) {
        Object* cube = &cubes[i];
        vec3 sign = cube->Translate * side;
        if ((sign[0] > 0) || (sign[1] > 0) || (sign[2] > 0)) {
            currentRotation.cubes.push_back({.cube = cube, .initialVector = cube->Translate, .initialRot = cube->Rotate});
        }
    }

    currentRotation.rotating = true;
}

void TurnRaw (string side, int direction) {
    for (unsigned int i = 0; i < 6; i++) {
        if (shortcuts[i] == side) {
            turn(sideOrder[i], direction);
            break;
        }
    }
};

int* read (vec3 side) {
    int num;

    for (unsigned int i = 0; i < 6; i++) {
        if (sideOrder[i] == side) {
            num = i;
        }
    }

    static int output[9];

    for (unsigned int i = 0; i < cubes.size(); i++) {
        Object *cube = &cubes[i];
        vec3 sign = cube->Translate * side;

        if ((sign[0] > 0) || (sign[1] > 0) || (sign[2] > 0)) {
            int* faces;
            vec3 directions[3] = {
                vec3 (0, 0, 1),
                vec3 (0, 1, 0),
                vec3 (-1, 0, 0),
            };

            faces = arrangement[cube->ID].faces;

            for (unsigned int j = 0; j < 3; j++) {
                vec3 direction = vec3(cube->Rotate * vec4(directions[j], 0.0f)) * side;

                if (round(length(direction)) > 0) {
                    for (unsigned int k = 0; k < 9; k++) {
                        if (faceOrder[num][k] == cube->Translate) {
                            output[k] = faces[j];
                        }
                    }
                }
            }
        }
    }

    return output;
}

int* Read (string side) {
    for (unsigned int i = 0; i < 6; i++) {
        if (shortcuts[i] == side) {
            return read(sideOrder[i]);
        }
    }

    cout<<"Couldn't find side"<<endl;
    static int dummy;
    return &dummy;
}

int state = 0;
struct operation {
    string face;
    int direction;
};
vector<operation> stack {};

void Turn(string face, int direction) {
    stack.push_back({.face = face, .direction = direction});
}

//1 is counterclockwise, -1 is clockwise
bool solve() {
    switch (state) {
        case 0: {//White cross
            int *colors = Read("y");
            int count = 0;

            for (int i = 0; i < 8; i++) {
                Read("w");
                if ((dictionary[colors[i]] == "w") && (i % 2 == 0)) {
                    string face = topbottom[i];
                    Read(face);

                    if (dictionary[colors[7]] != face) {
                        state = 1;
                        Turn(face, 1);
                        Turn(face, 1);
                    }
                }
            }

            for (int i = 0; i < 9; i++) {
                Read("y");
                if ((dictionary[colors[i]] == "w") && (i % 2 == 0)) {
                    count++;
                    string face = topbottom[i];
                    Read(face);
                    if (dictionary[colors[3]] == face) { //If the bottom white piece hits the matching color face
                        Turn(face, 1);
                        Turn(face, 1);
                    }
                }
            }

            if (count == 0) {
                for (int i = 0; i < 4; i++) {
                    Read(bias[i]);
                    if (dictionary[colors[5]] == "w") {
                        count++;
                        int check = i - 1;
                        if (check < 0) {
                            check = 3;
                        }

                        Read(bias[check]);
                        if (dictionary[colors[1]] == bias[check]) {
                            Turn(bias[check], 1); //Already on the right face
                        } else {
                            Turn(bias[check], -1); //Bring to bottom to be relocated
                            Turn("y", -1);
                            Turn(bias[check], 1);
                        }
                    }

                }

                if (count == 0) {
                    for (int i = 3; i >= 0; i--) {
                        Read(bias[i]);
                        if (dictionary[colors[1]] == "w") {
                            count++;
                            int check = i + 1;
                            if (check > 3) {
                                check = 0;
                            }

                            Read(bias[check]);
                            if (dictionary[colors[5]] == bias[check]) {
                                Turn(bias[check], -1); //Already on the right face
                            } else {
                                Turn(bias[check], 1); //Bring to bottom to be relocated
                                Turn("y", 1);
                                Turn(bias[check], -1);
                            }
                        }

                    }

                    if (count == -1) {
                        currentRotation.speed = 0.4f;

                        for (int i = 0; i < 4; i++) {
                            Read(bias[i]);
                            if (dictionary[colors[3]] == "w") {
                                string color;
                                for (int j = 0; j < 9; j++) {
                                    if (topbottom[j] == bias[i]) {
                                        Read("y");
                                        color = dictionary[colors[j]];
                                    }
                                }

                                if (color == bias[i]) {
                                    int check = i - 1;
                                    if (check < 0) {
                                        check = 3;
                                    }

                                    Turn(bias[i], -1);
                                    Turn(bias[check], -1);
                                    Turn("y", -1);
                                    Turn(bias[check], 1);
                                    Turn(bias[i], 1);
                                    Turn(bias[i], 1);
                                } else {
                                    count++;
                                }
                            } else if (dictionary[colors[7]] == "w") {
                                count++;
                                Turn(bias[i], 1);
                                Turn(bias[i], 1);
                            }
                        }

                        if (count == 0) {
                            state = 1;
                        }
                    }
                }
            } else if (stack.size() == 0) {
                Turn("y", 1);
            }

            break;
        } case 1: {
            break;
        }
    }

    return false;
}

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

    window = SDL_CreateWindow("Rubik's Cube \"Solver\"", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowX, windowY, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (window == NULL) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    glContext = SDL_GL_CreateContext(window);
    if (glContext == NULL) {
        std::cout << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_MULTISAMPLE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    vector <vec3> vertices;
    vector <vec2> uvs;
    vector <vec3> normals;

    bool res = LoadObjects("../cube.obj", vertices, uvs, normals);

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW);

    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW);

    GLuint normalbuffer;
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);

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

    for (unsigned int i = 0; i < 26; i++) {
        colorStore[i][0] = colors[arrangement[i].faces[0]];
        colorStore[i][1] = colors[arrangement[i].faces[1]];
        colorStore[i][2] = colors[arrangement[i].faces[2]];

        Object cube (i, arrangement[i].position, vec3(1, 1, 1), arrangement[i].rotation, colorStore[i]);
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

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        GLuint LightingID = glGetUniformLocation(programID, "LightDirection");
        GLuint AmbientID = glGetUniformLocation(programID, "Ambient");

        vec3 lightDirection = vec3(0.7, -1, 0.7);
        vec3 ambient = vec3(0.1, 0.1, 0.1);

        glUniform3fv(LightingID, 1, &lightDirection[0]);
        glUniform3fv(AmbientID, 1, &ambient[0]);

        //Camera projection
        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) windowX / windowY, 0.1f, 100.0f);

        //Camera matrix
        glm::mat4 View = lookAt(zoom * vec3(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle)), vec3 (0, 0, 0), vec3(0, 1, 0));

        if (currentRotation.rotating) {
            currentRotation.angle = fmin(currentRotation.angle + deltaTime * currentRotation.speed, (float) M_PI / 2);

            for (unsigned int i = 0; i < currentRotation.cubes.size(); i++) {
                cubeRotation info = currentRotation.cubes[i];
                mat4 newRotation = rotate(currentRotation.direction * currentRotation.angle, currentRotation.side);
                Object* cube = info.cube;
                cube->Translate = vec3(newRotation * vec4(info.initialVector, 0.0f));
                cube->Rotate = newRotation * info.initialRot;
            }

            if (currentRotation.angle == (float) M_PI / 2) {
                for (unsigned int i = 0; i < currentRotation.cubes.size(); i++) {
                    Object* cube = currentRotation.cubes[i].cube;
                    cube->Translate = vec3 (round(cube->Translate[0]), round(cube->Translate[1]), round(cube->Translate[2]));
                }

                currentRotation.rotating = false;
            }
        } else {
            if (!solving) {
                vec3 options[] = {
                    vec3(1, 0, 0),
                    vec3(-1, 0, 0),
                    vec3(0, 1, 0),
                    vec3(0, -1, 0),
                    vec3(0, 0, 1),
                    vec3(0, 0, -1),
                };

                int direction = -1;
                if ((rand() % 2) > 0.5f) {
                    direction = 1;
                }

                vec3 side = options[rand() % 6];

                turn(side, direction);
                scrambleCount += 1;

                if (scrambleCount > 20) {
                    currentRotation.speed = 4.0f;
                    scrambleCount = 0;
                    solving = true;
                }
            } else {
                if (stack.size() > 0) {
                    TurnRaw(stack[0].face, stack[0].direction);
                    stack.erase(stack.begin());
                } else {
                    solve();
                }
            }
        }
        for (unsigned int i = 0; i < cubes.size(); i++) {
            cubes[i].Draw(programID, Projection, View, vertices.size());
        };

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        SDL_GL_SwapWindow(window);
        i += 0.005;
    }

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();

    return 0;
}