#include "Events.hpp"
#include <cstdio>

// Comment so I can remember this function
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

		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		SwapBuffers(dc);

		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);
		}
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}