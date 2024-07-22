#pragma once

#include <Windows.h>
#include <glad/glad.h>

#include <STL-Reader.hpp>
#include <Window-Manager.hpp>
#include "Shader-Programs.hpp"

#include "Constants.hpp"


struct Material {
	glm::vec3 Mcolor;
	float Mambient, Mdiffuse, Mspecular, Mshininess;
};

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

class AppPtr {
public:
	AppPtr(void* ptr);
};

class App {
	bool appStatus = false;
public: 
	HINSTANCE instance;

	AppPtr ap;
	Man::Window window;

	// TODO: abstract all of this into one massive opengl class where all of this can be easily managed
	Man::FrameBuffer viewport;
	
	int swapInterval = -1;
	int prevSwapInterval = swapInterval;

	float moveSpeed = 0.005;
	float mouseSensitivity = 0.0035;

	int skyBoxAttribSizes[1] = { 3 };

	float skyBoxVerts[24] = {
		-1.0f, -1.0f,   1.0f, // 0 bottom   left    front
		 1.0f,  -1.0f,  1.0f, // 1 bottom   right   front
		-1.0f,  1.0f,   1.0f, // 2 top      left    front
		 1.0f,  1.0f,   1.0f, // 3 top      right   front
		-1.0f, -1.0f,  -1.0f, // 4 bottom   left    back
		 1.0f,  -1.0f, -1.0f, // 5 bottom   right   back
		-1.0f,  1.0f,  -1.0f, // 6 top      left    back
		 1.0f,  1.0f,  -1.0f  // 7 top      right   back
	};
	// TODO: try messing up the winding order
	// BOOKMARK: fix the winding order (Run the program)
	unsigned skyboxIndices[36] = {
		0, 2, 1, 1, 2, 3, // front
		2, 6, 3, 3, 6, 7, // top
		1, 3, 7, 7, 5, 1, // right
		2, 0, 4, 4, 6, 2, // left
		0, 1, 4, 4, 1, 5, // bottom
		4, 5, 6, 6, 5, 7, // back
	};

	STL meshFile;
	STL smoothedMesh;

	Man::Geometry skyBox;
	Man::Geometry mesh;
	float smoothCutoffAngle = 0.0f;
	float prevSmoothCutoffAngle = 0.0f;
	
	StlShader stlShdr; 
	SkyboxShader skyboxShdr;

	glm::vec3 cameraPos = { 0.0f, 0.0f, -10.0f };
	glm::vec2 cameraAngle = { 0.0f, 0.0f };
	glm::vec3 g_LightCol = { 1.0f, 1.0f, 1.0f };
	glm::vec3 g_LightPos = { 20.0f, 20.0f, 30.0f };
	glm::mat4 orbit = glm::mat4(1.0f); 
	Material crate = { { 0.4f, 0.6f, 0.1f }, 0.35f, 0.8f, 0.5f, 512.0f };
	glm::vec3 cratePos = { 0.0f, 0.0f, 0.0f };
	int shineExponent = 8;

	float sina = 0.0f, cosa = 1.0f;
	ULONGLONG prevTime = GetTickCount64();

	App(HINSTANCE hInstance, int nCmdShow) :
		instance(hInstance),
		ap(this),
		window(hInstance, nCmdShow, L"OpenGL Sandbox", winProc, 1.5, 0.9),
		viewport(window.getWindowWidth(), window.getWindowHeight()),
		meshFile(L"res/Meshes/Cable Reel High Res.stl"),
		skyBox(skyBoxVerts, 36, skyBoxAttribSizes, 1, 3 * sizeof(float), skyboxIndices, sizeof(skyboxIndices)),
		mesh((float*)meshFile.vertices.data(), meshFile.vertices.size(), StlVertexAttribSizes, 2, sizeof(StlVertex))
	{
		if (
			!window.getWindowStatus() ||
			!stlShdr.getShaderStatus() ||
			!viewport.getStatus()
			) 
		{
			appStatus = false;
			return;
		}
		else {
			appStatus = true;
		}

		window.showMaximized(); 
	}
	
	bool getAppStatus() { return appStatus; }

	/// After Initializing via the constructor all code is executed through the winProc.
	/// Rendering is handled in winProc -> WM_PAINT
	void run() {
		MSG msg = {};
		while (GetMessage(&msg, NULL, 0, 0) > 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg); 
		}
	}
};
extern App* aPtr;