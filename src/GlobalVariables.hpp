#pragma once

#include <Windows.h>
#include <dwmapi.h>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define CLEAR_COLOR_PARAM 0.05, 0.05, 0.05, 1.0

/// <summary>
/// The 4 values below get multiplied by the user set scale to determine the pixel values
/// </summary>
constexpr unsigned
c_WindowStartPosX = 200,
c_WindowStartPosY = 200,
c_WindowStartWidth = 1000,
c_WindowStartHeight = 800;

constexpr float c_MoveSensitivy = 0.002f;

constexpr float 
c_DefFov = 45.0f, // Default FOV (degrees)
c_NearClip = 0.1f,
c_FarClip = 100.0f;

/// <summary>
/// Determined and calculated
/// </summary>
extern unsigned windowWidth, windowHeight;

// General global variables:
extern ULONGLONG appStartTime;
extern ULONGLONG previousTime;
extern glm::mat4 model, view, proj;
extern glm::vec3 camPos;

// win32 global variables:
extern HWND hwnd;
extern HDC dc;
extern HGLRC glrc;
extern HANDLE consoleHandle;
extern MSG msg;

// OpenGL global variables;
extern GLint u_Model, u_View, u_Proj;