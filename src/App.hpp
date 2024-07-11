#pragma once

#include <Windows.h>
#include <glad/glad.h>


#include <Window-Manager.hpp>
#include "Shader-Programs.hpp"

#include "Constants.hpp"

struct Material {
	float Mambient;
	float Mdiffuse;
	float Mshininess;
};

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

class App {
	Vertex vertices[36] = {
		// back
		{ { -0.5f, -0.5f, -0.5f }, { 0.0f,  0.0f, -1.0f }, { 1.0f, 0.0f } },
		{ {  0.5f, -0.5f, -0.5f }, { 0.0f,  0.0f, -1.0f }, { 0.0f, 0.0f } },
		{ {  0.5f,  0.5f, -0.5f }, { 0.0f,  0.0f, -1.0f }, { 0.0f, 1.0f } },
		{ {  0.5f,  0.5f, -0.5f }, { 0.0f,  0.0f, -1.0f }, { 0.0f, 1.0f } },
		{ { -0.5f,  0.5f, -0.5f }, { 0.0f,  0.0f, -1.0f }, { 1.0f, 1.0f } },
		{ { -0.5f, -0.5f, -0.5f }, { 0.0f,  0.0f, -1.0f }, { 1.0f, 0.0f } },
		// front
		{ { -0.5f, -0.5f,  0.5f }, { 0.0f,  0.0f, 1.0f }, { 0.0f, 0.0f } },
		{ {  0.5f, -0.5f,  0.5f }, { 0.0f,  0.0f, 1.0f }, { 1.0f, 0.0f } },
		{ {  0.5f,  0.5f,  0.5f }, { 0.0f,  0.0f, 1.0f }, { 1.0f, 1.0f } },
		{ {  0.5f,  0.5f,  0.5f }, { 0.0f,  0.0f, 1.0f }, { 1.0f, 1.0f } },
		{ { -0.5f,  0.5f,  0.5f }, { 0.0f,  0.0f, 1.0f }, { 0.0f, 1.0f } },
		{ { -0.5f, -0.5f,  0.5f }, { 0.0f,  0.0f, 1.0f }, { 0.0f, 0.0f } },
		// left
		{ { -0.5f,  0.5f,  0.5f }, { -1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f } },
		{ { -0.5f,  0.5f, -0.5f }, { -1.0f,  0.0f,  0.0f }, { 0.0f, 1.0f } },
		{ { -0.5f, -0.5f, -0.5f }, { -1.0f,  0.0f,  0.0f }, { 0.0f, 0.0f } },
		{ { -0.5f, -0.5f, -0.5f }, { -1.0f,  0.0f,  0.0f }, { 0.0f, 0.0f } },
		{ { -0.5f, -0.5f,  0.5f }, { -1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f } },
		{ { -0.5f,  0.5f,  0.5f }, { -1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f } },
		// right
		{ { 0.5f,  0.5f,  0.5f }, { 1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f } },
		{ { 0.5f,  0.5f, -0.5f }, { 1.0f,  0.0f,  0.0f }, { 0.0f, 1.0f } },
		{ { 0.5f, -0.5f, -0.5f }, { 1.0f,  0.0f,  0.0f }, { 0.0f, 0.0f } },
		{ { 0.5f, -0.5f, -0.5f }, { 1.0f,  0.0f,  0.0f }, { 0.0f, 0.0f } },
		{ { 0.5f, -0.5f,  0.5f }, { 1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f } },
		{ { 0.5f,  0.5f,  0.5f }, { 1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f } },
		// bottom
		{ { -0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f,  0.0f }, { 0.0f, 0.0f } },
		{ {  0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f,  0.0f }, { 1.0f, 0.0f } },
		{ {  0.5f, -0.5f,  0.5f }, { 0.0f, -1.0f,  0.0f }, { 1.0f, 1.0f } },
		{ {  0.5f, -0.5f,  0.5f }, { 0.0f, -1.0f,  0.0f }, { 1.0f, 1.0f } },
		{ { -0.5f, -0.5f,  0.5f }, { 0.0f, -1.0f,  0.0f }, { 0.0f, 1.0f } },
		{ { -0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f,  0.0f }, { 0.0f, 0.0f } },
		// top
		{ { -0.5f,  0.5f, -0.5f }, { 0.0f,  1.0f,  0.0f }, { 0.0f, 0.0f } },
		{ {  0.5f,  0.5f, -0.5f }, { 0.0f,  1.0f,  0.0f }, { 1.0f, 0.0f } },
		{ {  0.5f,  0.5f,  0.5f }, { 0.0f,  1.0f,  0.0f }, { 1.0f, 1.0f } },
		{ {  0.5f,  0.5f,  0.5f }, { 0.0f,  1.0f,  0.0f }, { 1.0f, 1.0f } },
		{ { -0.5f,  0.5f,  0.5f }, { 0.0f,  1.0f,  0.0f }, { 0.0f, 1.0f } },
		{ { -0.5f,  0.5f, -0.5f }, { 0.0f,  1.0f,  0.0f }, { 0.0f, 0.0f } },
	};
	bool appStatus = true;

public: 
	Man::Window window;
	Man::Geometry box;
	ObjectShader objShdr;
	LightShader lghtShdr;


	glm::vec3 cameraPos = { 0.0f, 0.0f, -3.0f };
	glm::vec2 cameraAngle = { 0.0f, 0.0f };
	glm::mat4 proj;
	glm::mat4 view = glm::translate(glm::mat4(1.0f), cameraPos);
	glm::vec3 g_LightCol = { 1.0f, 1.0f, 1.0f };
	glm::vec3 g_LightPos = { 2.0f, 2.0f, 3.0f };
	Material crate = { 0.35f, 0.8f, 1024.0f };
	glm::vec3 cratePos = { 0.0f, 0.0f, 0.0f };
	glm::vec3 crateRotateVec = { 1.0f, 1.0f, -1.0f };
	float crateAngle = 0.0f;
	int shineExponent = 8;

	float sina = 0.0f, cosa = 1.0f;
	ULONGLONG prevTime = GetTickCount64();

	App(HINSTANCE hInstance, int nCmdShow)
		: window(hInstance, nCmdShow, c_WindowWidth, c_WindowHeight, L"Opengl Sandbox", winProc),
		box(vertices, sizeof(vertices) / sizeof(Vertex))
	{
		if (
			!window.getWindowStatus() ||
			!objShdr.getShaderStatus() ||
			!lghtShdr.getShaderStatus()
			) {
			appStatus = false;
			return;
		}
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