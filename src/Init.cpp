#include "GlobalVariables.hpp"
#include "init.hpp"

void initConsole() {
	AllocConsole();

	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetStdHandle(STD_INPUT_HANDLE, consoleHandle);
	SetStdHandle(STD_OUTPUT_HANDLE, consoleHandle);
	SetStdHandle(STD_ERROR_HANDLE, consoleHandle);

	FILE* file = nullptr;
	freopen_s(&file, "conin$", "r", stdin);
	freopen_s(&file, "conout$", "w", stdout);
	freopen_s(&file, "conout$", "w", stderr);
}
float getInitMonitorScale() {
	HMONITOR mntr = MonitorFromPoint({ 0, 0 }, MONITOR_DEFAULTTOPRIMARY);
	UINT dpiX, dpiY;
	GetDpiForMonitor(mntr, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
	return (float)(dpiX / 96.0);
}
bool initWindow(HINSTANCE hInstance, int nCmdShow) {
	SetProcessDPIAware();

	const wchar_t CLASS_NAME[] = L"WINDOW CLASS";

	WNDCLASS wc = { };

	wc.lpfnWndProc = windowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	float scale = getInitMonitorScale();
	windowWidth = (unsigned)(c_WindowStartWidth * scale);
	windowHeight = (unsigned)(c_WindowStartHeight * scale);
	hwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"Learn to Program Windows",
		WS_OVERLAPPEDWINDOW,
		int(c_WindowStartPosX * scale),
		int(c_WindowStartPosY * scale),
		windowWidth,
		windowHeight,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hwnd == NULL) { return false; }

	//ShowWindow(hwnd, nCmdShow);
	return true;
}
bool initOpenGL(int nCmdShow) {
	PIXELFORMATDESCRIPTOR pfd;
	int pFormat;

	pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0, 0, 0, 0,
		24,
		8,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
	dc = GetDC(hwnd);
	pFormat = ChoosePixelFormat(dc, &pfd);
	if (!pFormat) return false; 
	SetPixelFormat(dc, pFormat, &pfd);
	glrc = wglCreateContext(dc);
	wglMakeCurrent(dc, glrc);

	if (!gladLoadGL()) return false;

	glClearColor(CLEAR_COLOR_PARAM);
	glClear(GL_COLOR_BUFFER_BIT);
	SwapBuffers(dc);
	ShowWindow(hwnd, nCmdShow);

	return true;
}

bool initAll(HINSTANCE hInstance, int nCmdShow) {
	initConsole();
	if (!initWindow(hInstance, nCmdShow)) return false;
	if (!initOpenGL(nCmdShow)) return false;
	return true;
}