#ifndef UNICODE
#define UNICODE
#endif

#include "GlobalVariables.hpp"
#include "Init.hpp"

void closeApp();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	if (!initAll(hInstance, nCmdShow)) { return -1; }

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		glClear(GL_COLOR_BUFFER_BIT);
		SwapBuffers(dc);
	}
	closeApp();

	return 0;  
}

void closeApp() {
	FreeConsole();
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(glrc);
	ReleaseDC(hwnd, dc); 
}