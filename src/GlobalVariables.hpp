#pragma once

#include <Windows.h>

#include <glad/glad.h>

#define CLEAR_COLOR_PARAM 0.05, 0.05, 0.05, 1.0

/// <summary>
/// The 4 values below get multiplied by the user set scale to determine the pixel values
/// </summary>
constexpr unsigned
c_WindowStartPosX = 200,
c_WindowStartPosY = 200,
c_WindowStartWidth = 900,
c_WindowStartHeight = 700;


/// <summary>
/// Determined and calculated
/// </summary>
extern unsigned windowWidth, windowHeight;

// win32 global variables:
extern HWND hwnd;
extern HDC dc;
extern HGLRC glrc;