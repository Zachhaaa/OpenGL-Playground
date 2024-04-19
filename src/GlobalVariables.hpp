#pragma once

#include <Windows.h>
#include <dwmapi.h>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define CLEAR_COLOR_PARAM 0.05f, 0.05f, 0.05f, 1.0f

namespace pug {
	struct vec2s {
		short x, y;
	};
	struct vec2f {
		float x, y;
	};
	struct vec3f {
		float x, y, z;
	};
};

/// <summary>
/// The 6 values below get multiplied by the user set scale to determine the pixel values
/// </summary>
constexpr unsigned
c_WindowStartPosX   = 200,
c_WindowStartPosY   = 200,
c_WindowStartWidth  = 1000,
c_WindowStartHeight = 800,
c_WindowMinWidth    = 200,
c_WindowMinHeight   = 200;

/// <summary>
/// -1: Adaptive vsync
/// 0: SwapBuffers as many times as possible
/// x > 0: Sait until the monitor refershs x times before swapping the buffers
/// </summary>
constexpr int c_SwapInterval = -1;

// Control constants
constexpr float c_MoveSensitivity  = 0.0035f;
constexpr float c_MouseSensitivity = 0.002f;

constexpr float
c_DefFov = 45.0f, // Default FOV (degrees)
c_NearClip = 0.1f,
c_FarClip = 100.0f;

/// <summary>
/// Determined and calculated
/// </summary>
extern unsigned 
windowWidth,
windowHeight,
windowMinWidth,
windowMinHeight;

// General global variables:
extern ULONGLONG appStartTime;
extern ULONGLONG previousTime;
extern glm::mat4 model, view, proj;

// Camera
extern glm::vec3 camPos;
// NOTE: camAng.y refers to the rotation around the cameras y axis 
// For ex. increasing camAng.y turns the player view left (OpenGL coordinate system)
extern pug::vec2f camAng;

// win32 global variables:
extern HWND hwnd;
extern HDC dc;
extern HGLRC glrc;
extern HANDLE consoleHandle;
extern MSG msg;
extern bool windowFocus;

// OpenGL global variables;
extern GLint u_Model, u_View, u_Proj;