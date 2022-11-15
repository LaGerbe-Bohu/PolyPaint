# CMakeList.txt : CMake project for PolyPaint, include source and define
# project specific logic here.
#

# Add source to this project's executable.

add_executable (PolyPaint "PolyPaint.cpp" "PolyPaint.h" "GLShader.cpp")


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET PolyPaint PROPERTY CXX_STANDARD 20)
endif()


# TODO: Add tests and install targets if needed.
install(TARGETS PolyPaint DESTINATION  "PolyPaint/bin")




include_directories(../libs/include/GLFW/include)       
include_directories(../libs/include/GLEW/include)


link_directories(../libs/include/GLFW/lib-static-ucrt )
link_directories(../libs/include/GLEW/lib/Release/Win32)


find_package(OpenGL REQUIRED)


target_link_libraries(PolyPaint glfw3 ${OPENGL_gl_LIBRARY} glew32s )
