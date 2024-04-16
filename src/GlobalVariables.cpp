#include "GlobalVariables.hpp"

unsigned windowWidth = 0, windowHeight = 0;

// General global variables:
ULONGLONG appStartTime;
ULONGLONG previousTime;
glm::mat4 model, view, proj;
glm::vec3 camPos(0.0f, 0.0f, -3.0f);

// win32 global variables:
HWND hwnd;
HDC dc;
HGLRC glrc;
HANDLE consoleHandle;
MSG msg = {};

// OpenGL global variables"
GLint u_Model, u_View, u_Proj;