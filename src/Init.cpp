#include "GlobalVariables.hpp"
#include "init.hpp"

void initConsole() {
	AllocConsole();

	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
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

	return true;
}
/// <param name="shader">Shader to querry for errors</param>
/// <returns>0 if fails. Return value obtained from glGetShaderiv</returns>
GLint queryShaderErrors(GLuint shader) {
	GLint success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("SHADER ERROR:%s\n", infoLog);
	}
	return success; 
}
GLint queryProgramErrors(GLuint program) {
	GLint success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		printf("PROGRAM ERROR:%s\n", infoLog);
	}
	return success;
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

	float vertices[]{
		-0.5f, -0.5,  0.0f,
		 0.0f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f
	};
	GLuint vertexBuffer;
	GLuint vertexArray;
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray); 
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
	glEnableVertexAttribArray(0); 

	const char* vertexShaderSource =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main() {\n"
		"  gl_Position = vec4(aPos, 1.0);\n"
		"}";
	const char* fragmentShaderSource =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
		"  FragColor = vec4(0.88, 0.56, 0.19, 1.0);\n"
		"}";
	GLuint vertexShader, fragmentShader, shaderProgram;
	vertexShader   = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	shaderProgram  = glCreateProgram();
	if (!vertexShader || !fragmentShader || !shaderProgram) return false;

	glShaderSource(vertexShader  , 1, &vertexShaderSource  , NULL);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader); 
	GLint vertexShaderSuccess   = queryShaderErrors(vertexShader);
	GLint fragmentShaderSuccess = queryShaderErrors(fragmentShader);
	if (!vertexShaderSuccess || !fragmentShaderSuccess) return false;

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);
	queryProgramErrors(shaderProgram);

	glUseProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return true;
}

bool initAll(HINSTANCE hInstance, int nCmdShow) {
	appStartTime = GetTickCount64();
	initConsole();
	if (!initWindow(hInstance, nCmdShow)) return false;
	if (!initOpenGL(nCmdShow)) return false;
	return true;
}