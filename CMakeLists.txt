cmake_minimum_required(VERSION 3.12)
project(Physics)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGl REQUIRED)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

find_library(SDL2_FRAMEWORK SDL2)
add_executable(Physics main.cpp loadShader.cpp shader.h loadObject.cpp)
target_link_libraries(Physics libGLEW.a ${OPENGL_LIBRARIES})
target_link_libraries(Physics ${SDL2_FRAMEWORK})