//
// Created by Kenji Shore on 2019-01-06.
//

#ifndef PHYSICS_OBJECT_H
#define PHYSICS_OBJECT_H

#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;

class Object {
    mat4 Translate;
    mat4 Scale;
    mat4 Rotate;
    vec3* Colors;

    public:
        Object (vec3, vec3, mat4, vec3[3]);
        void Draw(GLuint, mat4, mat4, GLuint);

};


#endif //PHYSICS_OBJECT_H
