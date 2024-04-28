#pragma once

#include <Windows.h>
#include <dwmapi.h>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>

#ifdef _DEBUG
#define DEBUG_ONLY(x) x
#define GL_ERROR(x) x; \
{ \
	GLenum glError = glGetError(); \
	if (glError) {	\
		printf("OpenGl ERROR: %X\n", glError); \
		__debugbreak(); \
	} \
}
#else
#define  DEBUG_ONLY(X)
#define GL_ERROR(x) x
#endif

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
constexpr float c_MoveSensitivity  = 0.0025f;
constexpr float c_MouseSensitivity = 0.002f;

constexpr float
c_DefFov = 45.0f, // Default FOV (degrees)
c_NearClip = 0.1f,
c_FarClip = 100.0f;

// Scene Constants
constexpr pug::vec3f lightCol = { 1.0f,  1.0f,  1.0f  };
constexpr pug::vec3f objCol   = { 0.95f, 0.42f, 0.21f };
constexpr glm::vec3  lightPos(2.0f, 3.0f, 4.0f);
/// <summary>
/// Determined and calculated
/// </summary>
extern unsigned 
windowWidth,
windowHeight,
windowMinWidth,
windowMinHeight;
extern float scale;

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
extern GLuint objProg, lightProg;
extern GLint
u_ObjModel, u_ObjView, u_ObjProj, u_ObjObjCol, u_ObjLightCol, u_LightPos, u_ViewPos,
u_LightModel, u_LightView, u_LightProj, u_LightLightCol;
