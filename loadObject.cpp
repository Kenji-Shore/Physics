//
// Created by Kenji Shore on 2019-01-03.
//

#include <fstream>
#include <GL/glew.h>
#include <SDL2/SDL.h>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

using namespace std;

bool LoadObjects(const char* path, vector <glm::vec3> &out_vertices, vector <glm::vec2> &out_uv, vector <glm::vec2> &out_normals) {
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("Impossible to open the file /n");
        return false;
    }

    while (true) {
        char lineHeader[128];

        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF) {
            break;
        }
    }

    return false;
}