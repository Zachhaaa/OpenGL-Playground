#include "Events.hpp"
#include "Render.hpp"

#include <windowsx.h>

#include <cstdio>

// Comment so I can remember this function:
// SetConsoleCursorPosition(consoleHandle, { 0, 0 });

POINT prevMousePos;
pug::vec3f keyVec = { 0.0f, 0.0f, 0.0f };

LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		windowWidth  = LOWORD(lParam);
		windowHeight = HIWORD(lParam);
		glViewport(0, 0, windowWidth, windowHeight);

		proj = glm::perspective(glm::radians(c_DefFov), (float)windowWidth / windowHeight, c_NearClip, c_FarClip);
		glUniformMatrix4fv(u_Proj, 1, GL_FALSE, &proj[0][0]); 
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
	{
		/*if (lParam & 1 << 30) return 0;

		static float x = 0.0f, z = 0.0f;
		keyVec.y = 0.0f;*/

		switch (wParam) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		/*case 'W':
			z += 1.0f;
			break;
		case 'A':
			x += 1.0f;
			break;
		case 'S':
			z += -1.0f;
			break;
		case 'D':
			x += -1.0f;
			break;
		case VK_SPACE:
			keyVec.y += -1.0f;
			break;
		case VK_LSHIFT:
			keyVec.y += 1.0f;
			break;
		default:
			return 0;*/
		}
		/*float sina = sin(camAng.y), cosa = sin(camAng.x);
		keyVec.x = x * cosa - z * sina; 
		keyVec.z = x * sina + z * cosa;*/

		return 0;
	}
	case WM_MOUSEMOVE:
		if (GetAsyncKeyState(VK_MBUTTON) & 0x8000) {
			camAng.y += c_MouseSensitivity * (GET_X_LPARAM(lParam) - prevMousePos.x);
			camAng.x += c_MouseSensitivity * (GET_Y_LPARAM(lParam) - prevMousePos.y);
		}
		prevMousePos.x = GET_X_LPARAM(lParam);
		prevMousePos.y = GET_Y_LPARAM(lParam);
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