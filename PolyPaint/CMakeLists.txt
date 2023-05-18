

cmake_minimum_required(VERSION 3.6)
project(PolyPaint)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_VERBOSE_MAKEFILE 1)
set(APP_VERSION_MAJOR 1)
set(APP_VERSION_MINOR 0)


# glew
include_directories(../libs/GLEW/include)
link_directories(../libs/GLEW/lib/Release/x64)

# glfw
include_directories(../libs/GLFW/include)     
link_directories(../libs/GLFW/lib-static-ucrt )


add_subdirectory(imgui)
include_directories(${PROJECT_SOURCE_DIR})
add_executable (PolyPaint "PolyPaint.cpp"  "PolyPaint.h"  "GLShader.cpp" "coroutine.cpp")

target_link_libraries(PolyPaint imgui)



target_link_libraries(PolyPaint imgui  ${OPENGL_gl_LIBRARY} glfw3dll glew32s opengl32)