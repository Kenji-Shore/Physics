//
// Created by Kenji Shore on 2019-01-06.
//

#include "Object.h"

using namespace glm;
using namespace std;

Object::Object(vec3 newTranslate, vec3 newScale, mat4 newRotate, vec3 newColors[3]) {
    Translate = translate(newTranslate);
    Scale = scale(newScale);
    Rotate = newRotate;
    Colors = newColors;
}

void Object::Draw(GLuint programID, mat4 Projection, mat4 View, GLuint vertexCount) {
    mat4 Model = Translate * Rotate * Scale;
    mat4 mvp = Projection * View * Model;

    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

    GLuint ColorsID = glGetUniformLocation(programID, "Colors");
    glUniform3fv(ColorsID, 3, &Colors[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}