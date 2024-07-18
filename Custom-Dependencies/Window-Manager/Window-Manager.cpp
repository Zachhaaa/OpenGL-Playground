#include "Window-Manager.hpp"

Man::Window::Window(HINSTANCE hInstance, int nCmdShow, int windowWidth, int windowHeight, const wchar_t* windowTitle, WNDPROC winProc)
	: nCmdShow(nCmdShow)
{
	SetProcessDPIAware();

	WNDCLASS wc = {};
	wc.lpfnWndProc = winProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = windowTitle;

	RegisterClass(&wc);

	HMONITOR mntr = MonitorFromPoint({ 0, 0 }, MONITOR_DEFAULTTOPRIMARY);
	UINT dpiX, dpiY;
	GetDpiForMonitor(mntr, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
	float scale = float(dpiX / 96.0);

	hwnd = CreateWindowEx(
		0,
		windowTitle,
		windowTitle,
		WS_OVERLAPPEDWINDOW,
		300,
		300,
		int(windowWidth * scale),
		int(windowHeight * scale),
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (hwnd == NULL) {
		windowStatus = false;
		__debugbreak();
	}

	SetCursor(LoadCursor(NULL, IDC_ARROW));

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
	if (dc == NULL) {
		windowStatus = false;
		__debugbreak();
	}
	pFormat = ChoosePixelFormat(dc, &pfd);
	if (pFormat == NULL) {
		windowStatus = false;
		__debugbreak();
	}
	SetPixelFormat(dc, pFormat, &pfd);
	glrc = wglCreateContext(dc);
	wglMakeCurrent(dc, glrc);

	if (!gladLoadGL()) {
		windowStatus = false;
		__debugbreak();
	}

	wglSwapIntervalEXT = (WGLSWAPINTERVALEXT)wglGetProcAddress("wglSwapIntervalEXT");
	wglSwapIntervalEXT(-1);
	GL_ERROR(glEnable(GL_CULL_FACE));
	GL_ERROR(glCullFace(GL_BACK));
	GL_ERROR(glFrontFace(GL_CCW));
	GL_ERROR(glEnable(GL_DEPTH_TEST));
	GL_ERROR(glClearColor(0.0f, 0.0f, 0.0f, 0.1f));

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplOpenGL3_Init();

	io.Fonts->Clear();
	io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/arial.ttf", scale * 16.0f);
	io.Fonts->Build();

	//RECT rcClient;
	//GetWindowRect(hwnd, &rcClient);

	//// Inform the application of the frame change.
	//SetWindowPos(hwnd,
	//	NULL,
	//	rcClient.left, rcClient.top,
	//	rcClient.left - rcClient.right, rcClient.bottom - rcClient.top,
	//	SWP_FRAMECHANGED);
}