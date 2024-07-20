#include "Window-Manager.hpp"

/// To maximize a window, set percentSize = 1.0
/// @param hInstace WinMain hIstance
/// @param nCmdShow WinMain nCmdShow
/// @param aspectRatio Window aspect ratio (width / height)
/// @param percentSize = MonitorWidth(px) * percentSize or MonitorWidth(px) * percentSize. Smaller value is used and the other width is calculated based on the aspect ratio. 1.0 maximizes the window and aspect ratio is ignored.
Man::Window::Window(HINSTANCE hInstance, int nCmdShow, const wchar_t* windowTitle, WNDPROC winProc, float aspectRatio, float percentSize)
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

	int mntrWidth  = GetSystemMetrics(SM_CXSCREEN);
	int mntrHeight = GetSystemMetrics(SM_CYSCREEN);
	if (mntrWidth > mntrHeight) {
		windowHeight = percentSize * mntrHeight;
		windowWidth = aspectRatio  * windowHeight;
	}
	else {
		windowWidth = percentSize * mntrWidth;
		windowHeight = windowWidth / aspectRatio; 
	}
	int startPosX = 0.5 * (mntrWidth  - windowWidth);
	int startPosY = 0.5 * (mntrHeight - windowHeight);

	hwnd = CreateWindowEx(
		0,
		windowTitle,
		windowTitle,
		WS_OVERLAPPEDWINDOW,
		startPosX,
		startPosY,
		windowWidth,
		windowHeight,
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