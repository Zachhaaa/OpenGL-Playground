#include "GlobalVariables.hpp"
#include "init.hpp"

BOOL (WINAPI* wglSwapIntervalEXT)(int);

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
	windowWidth     = (unsigned)(c_WindowStartWidth  * scale);
	windowHeight    = (unsigned)(c_WindowStartHeight * scale);
	windowMinWidth  = (unsigned)(c_WindowMinWidth    * scale);
	windowMinHeight = (unsigned)(c_WindowMinHeight   * scale);

	hwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		L"Win32OpenGL",
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

	if (hwnd == NULL) { 
		DEBUG_ONLY(printf("Window handle: hwnd, failed. hwnd = NULL\n"); __debugbreak());
		return false; 
	}

	SetCursor(LoadCursor(NULL, IDC_ARROW));

	return true;
}
/// <param name="shader">Shader to querry for errors</param>
/// <returns>0 if fails. Return value obtained from glGetShaderiv</returns>
GLint queryShaderErrors(GLuint shader) {
	GLint success;
	DEBUG_ONLY(char infoLog[512]);
	GL_ERROR(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
	
	DEBUG_ONLY(
	if (!success) {
		GL_ERROR(glGetShaderInfoLog(shader, 512, NULL, infoLog));
		printf("SHADER ERROR:%s\n", infoLog);
	}
	)
	return success; 
}
GLint queryProgramErrors(GLuint program) {
	GLint success;
	DEBUG_ONLY(char infoLog[512]);
	GL_ERROR(glGetProgramiv(program, GL_LINK_STATUS, &success));

	DEBUG_ONLY(
	if (!success) {
		GL_ERROR(glGetProgramInfoLog(program, 512, NULL, infoLog));
		printf("PROGRAM ERROR:%s\n", infoLog);
	}
	)
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
	if (!pFormat) {
		DEBUG_ONLY(printf("pFormat error. ChoosePixelFormat() function returned 0\n"); __debugbreak());
		return false;
	}
	SetPixelFormat(dc, pFormat, &pfd);
	glrc = wglCreateContext(dc);
	wglMakeCurrent(dc, glrc);

	if (!gladLoadGL()) {
		DEBUG_ONLY(printf("gladLoadGL() function returned 0\n"); __debugbreak());
		return false;
	}

	wglSwapIntervalEXT = (WGLSWAPINTERVALEXT)wglGetProcAddress("wglSwapIntervalEXT");

	GL_ERROR(glClearColor(CLEAR_COLOR_PARAM));
	GL_ERROR(glEnable(GL_DEPTH_TEST));
	GL_ERROR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	SwapBuffers(dc);

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};
	GLuint vertexBuffer;
	GLuint objectVA;
	GL_ERROR(glGenVertexArrays(1, &objectVA));
	GL_ERROR(glBindVertexArray(objectVA));
	GL_ERROR(glGenBuffers(1, &vertexBuffer));
	GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
	GL_ERROR(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

	GL_ERROR(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
	GL_ERROR(glEnableVertexAttribArray(0));


	const char* objVertShadSrc =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"uniform mat4 u_Model, u_View, u_Proj;\n"
		"void main() {\n"
		"  gl_Position = u_Proj * u_View * u_Model * vec4(aPos, 1.0);\n"
		"}\n";
	const char* objFragShadSrc =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
		"  FragColor = vec4(0.95, 0.42, 0.21, 1.0);\n"
		"}";
	const char* lightVertShadSrc =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"uniform mat4 u_Model, u_View, u_Proj;\n"
		"void main() {\n"
		"  gl_Position = u_Proj * u_View * u_Model * vec4(aPos, 1.0);\n"
		"}\n";
	const char* lightFragShadSrc =
		"#version 330 core\n"
		"out vec4 lightColor;\n"
		"void main() {\n"
		"  lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
		"}\n";
	GLuint objVertShad, objFragShad, lightVertShad, lightFragShad;
	objVertShad   = GL_ERROR(glCreateShader(GL_VERTEX_SHADER));
	objFragShad   = GL_ERROR(glCreateShader(GL_FRAGMENT_SHADER));
	lightVertShad = GL_ERROR(glCreateShader(GL_VERTEX_SHADER));
	lightFragShad = GL_ERROR(glCreateShader(GL_FRAGMENT_SHADER));
	objProg       = GL_ERROR(glCreateProgram());
	lightProg     = GL_ERROR(glCreateProgram());
	if (
		!objVertShad ||
		!objFragShad ||
		!lightVertShad ||
		!lightFragShad ||
		!objProg ||
		!lightProg
		) {
		DEBUG_ONLY(printf("glCreateShader() function failed\n"); __debugbreak());
		return false;
	}

	GL_ERROR(glShaderSource(objVertShad,      1, &objVertShadSrc, NULL));
	GL_ERROR(glShaderSource(objFragShad,   1, &objFragShadSrc,    NULL));
	GL_ERROR(glShaderSource(lightVertShad, 1, &lightVertShadSrc, NULL));
	GL_ERROR(glShaderSource(lightFragShad, 1, &lightFragShadSrc,  NULL));

	GL_ERROR(glCompileShader(objVertShad));
	GL_ERROR(glCompileShader(objFragShad));
	GL_ERROR(glCompileShader(lightVertShad)); 
	GL_ERROR(glCompileShader(lightFragShad));

	GLint vertShadSuccess  = queryShaderErrors(objVertShad);
	GLint fragShadSuccess  = queryShaderErrors(objFragShad);
	GLint lightVertSuccess = queryShaderErrors(lightVertShad);
	GLint lightShadSuccess = queryShaderErrors(lightFragShad);
	if (!vertShadSuccess ||
		!fragShadSuccess ||
		!lightVertSuccess ||
		!lightShadSuccess
		) {
		DEBUG_ONLY(__debugbreak());
		return false;
	}

	GL_ERROR(glAttachShader(objProg, objVertShad));
	GL_ERROR(glAttachShader(objProg, objFragShad));
	glLinkProgram(objProg);
	if (!queryProgramErrors(objProg)) {
		DEBUG_ONLY(__debugbreak());
		return false;
	}

	GL_ERROR(glAttachShader(lightProg, lightVertShad));
	GL_ERROR(glAttachShader(lightProg, lightFragShad));
	GL_ERROR(glLinkProgram(lightProg));
	if (!queryProgramErrors(lightProg)) {
		DEBUG_ONLY(__debugbreak());
		return false;
	}

	GL_ERROR(glDeleteShader(objVertShad));
	GL_ERROR(glDeleteShader(objFragShad));
	GL_ERROR(glDeleteShader(lightFragShad));
	GL_ERROR(glDeleteShader(lightFragShad));

	u_ObjModel   = GL_ERROR(glGetUniformLocation(objProg,   "u_Model"));
	u_ObjView    = GL_ERROR(glGetUniformLocation(objProg,   "u_View"));
	u_ObjProj    = GL_ERROR(glGetUniformLocation(objProg,   "u_Proj"));
	u_LightModel = GL_ERROR(glGetUniformLocation(lightProg, "u_Model"));
	u_LightView  = GL_ERROR(glGetUniformLocation(lightProg, "u_View"));
	u_LightProj  = GL_ERROR(glGetUniformLocation(lightProg, "u_Proj"));
	if (u_ObjModel   == -1 ||
		u_ObjView    == -1 || 
		u_ObjProj    == -1 ||
		u_LightModel == -1 ||
		u_LightView	 == -1 ||
		u_LightProj	 == -1 
		) {
		DEBUG_ONLY(printf("ERROR finding uniform location\n"); __debugbreak());
		return false;
	}
	proj = glm::perspective(glm::radians(c_DefFov), (float)windowWidth/windowHeight, c_NearClip, c_FarClip);

	GL_ERROR(glUseProgram(objProg);)
	GL_ERROR(glUniformMatrix4fv(u_ObjProj,   1, GL_FALSE, &proj[0][0]));

	GL_ERROR(glUseProgram(lightProg);)
	GL_ERROR(glUniformMatrix4fv(u_LightProj, 1, GL_FALSE, &proj[0][0]));

	ShowWindow(hwnd, nCmdShow);

	return true;
}

bool initAll(HINSTANCE hInstance, int nCmdShow) {
	appStartTime = previousTime = GetTickCount64();
	initConsole();
	if (!initWindow(hInstance, nCmdShow)) {
		DEBUG_ONLY(printf("Window initialization failed\n"); __debugbreak());
		return false;
	}
	if (!initOpenGL(nCmdShow)) {
		DEBUG_ONLY(printf("OpenGL initialization failed\n"); __debugbreak());
		return false;
	}
	return true;
}