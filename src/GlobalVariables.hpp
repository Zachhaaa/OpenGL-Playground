#pragma once

#include <Windows.h>
#include <dwmapi.h>

#include <glad/glad.h>

#define CLEAR_COLOR_PARAM 0.05, 0.05, 0.05, 1.0

/// <summary>
/// The 4 values below get multiplied by the user set scale to determine the pixel values
/// </summary>
constexpr unsigned
c_WindowStartPosX = 200,
c_WindowStartPosY = 200,
c_WindowStartWidth = 1000,
c_WindowStartHeight = 800;


/// <summary>
/// Determined and calculated
/// </summary>
extern unsigned windowWidth, windowHeight;

// General global variables:
extern ULONGLONG appStartTime;
extern float rotateMat[3][3];

// win32 global variables:
extern HWND hwnd;
extern HDC dc;
extern HGLRC glrc;
extern HANDLE consoleHandle;

// OpenGL global variables;