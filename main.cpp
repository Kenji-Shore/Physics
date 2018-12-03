#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>

int main() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    std::cout << "success!" << std::endl;

    return 0;
}