#include "GlobalVariables.hpp"

unsigned
windowWidth,
windowHeight,
windowMinWidth,
windowMinHeight;
float scale;

// Scene 
pug::vec3f lightCol = { 1.0f,  1.0f,  1.0f };
float 
objAmbient   = 0.35f,
objDiffuse   = 1.0f,
objShininess = 32.0f;
glm::vec3  lightPos(2.0f, 3.0f, 4.0f);

// General global variables:
ULONGLONG previousTime = GetTickCount64();
glm::mat4 model, view, proj;
bool quit = false;
bool isMinimized = false;

// Camera
glm::vec3 camPos(0.0f, 0.0f, -3.0f);
pug::vec2f camAng = { 0.0f, 0.0f };


// win32 global variables:
HWND hwnd;
HDC dc;
HGLRC glrc;
HANDLE consoleHandle;
MSG msg = {};
bool windowFocus = true;

// OpenGL global variables"
GLuint objProg, lightProg;
App::ObjUniforms ObjUni;
App::LightUniforms LightUni;