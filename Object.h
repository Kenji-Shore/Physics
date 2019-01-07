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
    public:
        vec3 Translate;
        vec3 Scale;
        mat4 Rotate;
        int ID;
        Object (int, vec3, vec3, mat4, vec3[3]);
        void Draw(GLuint, mat4, mat4, GLuint);
    private:
        vec3* Colors;
};


#endif //PHYSICS_OBJECT_H
