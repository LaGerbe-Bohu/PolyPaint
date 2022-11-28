// PolyPaint.h : Include file for standard system include files,
// or project specific include files.

#pragma once


#define GLEW_STATIC

# define M_PI           3.14159265358979323846  /* pi */




#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


#include <iostream>
#include <cmath>
#include <vector> 
#include<stack>
#include <cstdint>
#include "GLShader.h"


#include "SimMath.cpp"
#include "CustomUi.cpp"
#include <future>


static const float triangle[] = {
       -1.0f,-1.0f,
       1.0f,-1.0f,
       1.0f,1.0f,
       -1.0f,1.0f,
};

static const float uv[] = {
          0.0f,0.0f,
       1.0f,0.0f,
       1.0f,1.0f,
       0.0f,1.0f,

};

// TODO: Reference additional headers your program requires here.

void Initialize();
