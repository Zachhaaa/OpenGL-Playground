#include "Events.hpp"
#include "Render.hpp"

#include <windowsx.h>

#include <imgui_impl_win32.h>

#include <cstdio>

POINT prevMousePos = { 0, 0 };
pug::vec3f playerVec = { 0.0f, 0.0f, 0.0f };
float sina = 0.0f, cosa = 1.0f;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam)) return true;

	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		windowWidth  = LOWORD(lParam);
		windowHeight = HIWORD(lParam);
		GL_ERROR(glViewport(0, 0, windowWidth, windowHeight));

		proj = glm::perspective(glm::radians(c_DefFov), (float)windowWidth / windowHeight, c_NearClip, c_FarClip);

		GL_ERROR(glUseProgram(objProg));
		GL_ERROR(glUniformMatrix4fv(u_ObjProj, 1, GL_FALSE, &proj[0][0]));

		GL_ERROR(glUseProgram(lightProg));
		GL_ERROR(glUniformMatrix4fv(u_LightProj, 1, GL_FALSE, &proj[0][0]));
		render();

		return 0;
	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = windowMinWidth;
		lpMMI->ptMinTrackSize.y = windowMinHeight;
		return 0;
	}
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		return 0;
	case WM_SETFOCUS:
		windowFocus = true;
		return 0;
	case WM_KILLFOCUS:
		windowFocus = false;
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}