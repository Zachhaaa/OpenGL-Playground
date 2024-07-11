#ifndef UNICODE
#define UNICODE
#endif

#include "App.hpp"
#include <STL-Reader.hpp>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

	App app(hInstance, nCmdShow);
	aPtr = &app;
	app.window.show();

	app.run(); 

	return 0;  
}
