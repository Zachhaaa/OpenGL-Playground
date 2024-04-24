#include "GlobalVariables.hpp"

unsigned
windowWidth,
windowHeight,
windowMinWidth,
windowMinHeight;

// General global variables:
ULONGLONG appStartTime;
ULONGLONG previousTime;
glm::mat4 model, view, proj;

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
GLint
u_ObjModel, u_ObjView, u_ObjProj, u_ObjObjCol, u_ObjLightCol, u_LightPos, u_ViewPos,
u_LightModel, u_LightView, u_LightProj, u_LightLightCol;