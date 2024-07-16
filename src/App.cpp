
#define STB_IMAGE_IMPLEMENTATION
#include "App.hpp"
#include "Events-Render.hpp"

App* aPtr; 

LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam)) return true;

	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		windowResize(lParam);
		return 0;
	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = c_WindowMinWidth;  
		lpMMI->ptMinTrackSize.y = c_WindowMinHeight;
		return 0;
	}
	case WM_PAINT:
		render();
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
