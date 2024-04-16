#include "GlobalVariables.hpp"

unsigned windowWidth = 0, windowHeight = 0;

// General global variables:
ULONGLONG appStartTime;
float rotateMat[3][3];

// win32 global variables:
HWND hwnd;
HDC dc;
HGLRC glrc;
HANDLE consoleHandle;

// OpenGL global variables"