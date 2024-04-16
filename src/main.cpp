#ifndef UNICODE
#define UNICODE
#endif

#include "GlobalVariables.hpp"
#include "Init.hpp"
#include "Render.hpp"

inline void closeApp() {
	FreeConsole();
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(glrc);
	ReleaseDC(hwnd, dc);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	if (!initAll(hInstance, nCmdShow)) { return -1; }
	while (true) {
		if(processEvents()) break;
		render();
	}
	closeApp();
	return 0;  
}
