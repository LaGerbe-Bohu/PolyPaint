

cmake_minimum_required(VERSION 3.6)
project(PolyPaint)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_VERBOSE_MAKEFILE 1)
set(APP_VERSION_MAJOR 1)
set(APP_VERSION_MINOR 0)



add_subdirectory(imgui)
include_directories(${PROJECT_SOURCE_DIR})
add_executable (PolyPaint "PolyPaint.cpp"  "PolyPaint.h"  "GLShader.cpp")

target_link_libraries(PolyPaint imgui)

# glfw
include_directories(../libs/include/GLFW/include)     
link_directories(../libs/include/GLFW/lib-static-ucrt )


# glew
include_directories(../libs/include/GLEW/include)
link_directories(../libs/include/GLEW/lib/Release/Win32)

target_link_libraries(PolyPaint imgui glfw3 ${OPENGL_gl_LIBRARY} glew32s )