#include "Events.hpp"
#include "Render.hpp"

#include <cstdio>

// Comment so I can remember this function:
// SetConsoleCursorPosition(consoleHandle, { 0, 0 });


LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		windowWidth = LOWORD(lParam);
		windowHeight = HIWORD(lParam);
		glViewport(0, 0, windowWidth, windowHeight);

		proj = glm::perspective(glm::radians(c_DefFov), (float)(windowWidth/windowHeight), c_NearClip, c_FarClip);
		glUniformMatrix4fv(u_Proj, 1, GL_FALSE, &proj[0][0]); 
		
		render();

		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}