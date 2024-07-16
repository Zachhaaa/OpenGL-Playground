#ifndef UNICODE
#define UNICODE
#endif

#include "App.hpp"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

	App app(hInstance, nCmdShow);
	if (!app.getAppStatus()) { return 1; }\

	aPtr = &app;
	app.window.show();

	app.run(); 

	return 0;  
}
