cmake_minimum_required(VERSION 3.12)
project(RubiksCube)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGl REQUIRED)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

find_library(SDL2_FRAMEWORK SDL2)
add_executable(RubiksCube main.cpp Object.cpp Object.h)
target_link_libraries(RubiksCube libGLEW.a ${OPENGL_LIBRARIES})
target_link_libraries(RubiksCube ${SDL2_FRAMEWORK})