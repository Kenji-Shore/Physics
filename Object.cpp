//
// Created by Kenji Shore on 2019-01-06.
//

#include "Object.h"

using namespace glm;
using namespace std;

Object::Object(int newID, vec3 newTranslate, vec3 newScale, mat4 newRotate, vec3 newColors[3]) {
    ID = newID;
    Translate = newTranslate;
    Scale = newScale;
    Rotate = newRotate;
    Colors = newColors;
}

void Object::Draw(GLuint programID, mat4 Projection, mat4 View, GLuint vertexCount) {
    mat4 Model = translate(Translate) * Rotate * scale(Scale);
    mat4 mvp = Projection * View * Model;

    GLuint RotationID = glGetUniformLocation(programID, "ModelRotation");
    glUniformMatrix4fv(RotationID, 1, GL_FALSE, &Rotate[0][0]);

    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

    GLuint ColorsID = glGetUniformLocation(programID, "Colors");
    glUniform3fv(ColorsID, 3, &Colors[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}