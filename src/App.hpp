#pragma once

#include <Windows.h>
#include <glad/glad.h>

#include <STL-Reader.hpp>
#include <Window-Manager.hpp>
#include "Shader-Programs.hpp"
#include <chrono>

#include "Constants.hpp"


struct Material {
	glm::vec3 Mcolor;
	float Mambient, Mdiffuse, Mspecular, Mshininess;
};

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

class App {
	bool appStatus = true;
public: 
	HINSTANCE instance;
	Man::Window window;
	
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
	glm::mat4 proj;
	glm::mat4 view = glm::translate(glm::mat4(1.0f), cameraPos);
	glm::vec3 g_LightCol = { 1.0f, 1.0f, 1.0f };
	glm::vec3 g_LightPos = { 20.0f, 20.0f, 30.0f };
	glm::mat4 orbit = glm::mat4(1.0f); 
	Material crate = { { 0.4f, 0.6f, 0.1f }, 0.35f, 0.8f, 0.5f, 512.0f };
	glm::vec3 cratePos = { 0.0f, 0.0f, 0.0f };
	int shineExponent = 8;

	float sina = 0.0f, cosa = 1.0f;
	ULONGLONG prevTime = GetTickCount64();

	App(HINSTANCE hInstance, int nCmdShow)
		:
		instance(hInstance),
		window(hInstance, nCmdShow, c_WindowWidth, c_WindowHeight, L"Opengl Sandbox", winProc),
		meshFile(L"res/Meshes/Cable Reel(binary).stl"),
		mesh((float*)meshFile.vertices.data(), meshFile.vertices.size(), StlVertexAttribSizes, 2, sizeof(StlVertex))
	{
		if (
			!window.getWindowStatus() ||
			!stlShdr.getShaderStatus()
			) {
			appStatus = false;
			return;
		}
		stlShdr.bind();
	}

	bool getAppStatus() { return appStatus; }
	void run() {
		MSG msg = {};
		while (GetMessage(&msg, NULL, 0, 0) > 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg); 
		}
	}
};
extern App* aPtr;