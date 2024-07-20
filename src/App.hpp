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

	Man::FrameBuffer viewport;
	
	int swapInterval = -1;
	int prevSwapInterval = swapInterval;

	float moveSpeed = 0.005;
	float mouseSensitivity = 0.0035;

	STL meshFile;
	STL smoothedMesh;

	Man::Geometry mesh;
	float smoothCutoffAngle = 0.0f;
	float prevSmoothCutoffAngle = 0.0f;
	
	StlShader stlShdr; 

	glm::vec3 cameraPos = { 0.0f, -1.0f, -10.0f };
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
		window(hInstance, nCmdShow, L"OpenGL Sandbox", winProc, 1.5, 0.85),
		viewport(window.getWindowWidth() - 300, window.getWindowHeight() - 300),
		meshFile(L"res/Meshes/Cable Reel(binary).stl"),
		mesh((float*)meshFile.vertices.data(), meshFile.vertices.size(), StlVertexAttribSizes, 2, sizeof(StlVertex))
	{
		if (
			!window.getWindowStatus() ||
			!stlShdr.getShaderStatus() ||
			!viewport.getStatus()
			) {
			appStatus = false;
			return;
		}
		else {
			appStatus = true;
		}

		stlShdr.bind();
		window.show(); 
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