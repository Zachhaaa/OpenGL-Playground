#include "GlobalVariables.hpp"
#include "init.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

	scale = getInitMonitorScale();
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
		DEBUG_ONLY(printf("Window Creation failed\n"); __debugbreak());
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
GLint getUniform(GLuint prog, const char* name) {
	GLint loc = GL_ERROR(glGetUniformLocation(prog, name));
	DEBUG_ONLY(
	if(loc == -1) {
		printf("ERROR finding uniform location: %s\n", name); 
		__debugbreak(); 
	}
	)
	return loc;
}
bool initShader(GLuint prog, GLenum type, const wchar_t* fileName) {
	HANDLE file = CreateFile(
		fileName, 
		GENERIC_READ, 
		0, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	if (file == INVALID_HANDLE_VALUE) {
		DEBUG_ONLY(wprintf(L"Cant open: %s\n", fileName); __debugbreak());
		return false; 
	}
	unsigned long long fileSize;
	GetFileSizeEx(file, (PLARGE_INTEGER)&fileSize);
	char* shaderSrc = new char[fileSize + 1]; 

	DWORD bytesRead;
	ReadFile(file, shaderSrc, fileSize, &bytesRead, NULL); 
	shaderSrc[bytesRead] = '\0'; 

	CloseHandle(file); 



	GLuint shdr = GL_ERROR(glCreateShader(type));
	if (!shdr) {
		DEBUG_ONLY(__debugbreak());
		return false;
	}
	GL_ERROR(glShaderSource(shdr, 1, &shaderSrc, NULL));
	GL_ERROR(glCompileShader(shdr));

	if (!queryShaderErrors(shdr)) {
		DEBUG_ONLY(__debugbreak());
		return false;
	}

	GL_ERROR(glAttachShader(prog, shdr));

	GL_ERROR(glDeleteShader(shdr));
	delete[] shaderSrc; 
	return true;
}
bool initTexture(GLenum textureSlot, const char* file) {
	GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GL_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	int width, height, nrChannels;
	unsigned char* img = stbi_load(file, &width, &height, &nrChannels, 0);
	
	if (!img) {
		DEBUG_ONLY(printf("texture image load failure\n"); __debugbreak());
		return false;
	}
	if (nrChannels != 4) {
		DEBUG_ONLY(printf("texture image must have 4 channels\n"); __debugbreak());
		return false;
	}
	unsigned int texture;
	GL_ERROR(glActiveTexture(textureSlot));
	GL_ERROR(glGenTextures(1, &texture));
	GL_ERROR(glBindTexture(GL_TEXTURE_2D, texture));

	GL_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img));
	GL_ERROR(glGenerateMipmap(GL_TEXTURE_2D));
	stbi_image_free(img);

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
	wglSwapIntervalEXT(-1);

	GL_ERROR(glClearColor(CLEAR_COLOR_PARAM));
	GL_ERROR(glEnable(GL_DEPTH_TEST));
	GL_ERROR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	SwapBuffers(dc);

	// NOTE: 
	// Normal Vectors must be normalized
	// 
	// Make sure to change the glVertexAttribPointer functions accordingly to match the data
	// in the vertices array

	float vertices[] = {
		// back
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
		// front
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
		// left
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		// right
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		 // bottom
		 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
		  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		 -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
		 // top
		 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
		  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
	};
	GLuint vertexBuffer;
	GLuint objectVA;
	GL_ERROR(glGenVertexArrays(1, &objectVA));
	GL_ERROR(glBindVertexArray(objectVA));
	GL_ERROR(glGenBuffers(1, &vertexBuffer));
	GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
	GL_ERROR(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

	GL_ERROR(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
	GL_ERROR(glEnableVertexAttribArray(0));
	GL_ERROR(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
	GL_ERROR(glEnableVertexAttribArray(1));
	GL_ERROR(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))));
	GL_ERROR(glEnableVertexAttribArray(2));

	objProg = GL_ERROR(glCreateProgram());
	lightProg = GL_ERROR(glCreateProgram());
	if (
		!initShader(objProg, GL_VERTEX_SHADER, L"res/Shaders/Object-Shader.vert") ||
		!initShader(objProg, GL_FRAGMENT_SHADER, L"res/Shaders/Object-Shader.frag") ||
		!initShader(lightProg, GL_VERTEX_SHADER, L"res/Shaders/Light-Shader.vert") ||
		!initShader(lightProg, GL_FRAGMENT_SHADER, L"res/Shaders/Light-Shader.frag")
		) {
		return false;
	}

	GL_ERROR(glLinkProgram(objProg));
	if (!queryProgramErrors(objProg)) {
		DEBUG_ONLY(__debugbreak());
		return false;
	}
	GL_ERROR(glLinkProgram(lightProg));
	if (!queryProgramErrors(lightProg)) {
		DEBUG_ONLY(__debugbreak());
		return false;
	}

	ObjUni.u_Model = getUniform(objProg, "u_Model");
	ObjUni.u_View = getUniform(objProg, "u_View");
	ObjUni.u_Proj = getUniform(objProg, "u_Proj");
	ObjUni.u_LightCol = getUniform(objProg, "u_LightCol");
	ObjUni.u_LightPos = getUniform(objProg, "u_LightPos");
	ObjUni.u_ViewPos = getUniform(objProg, "u_ViewPos");

	ObjUni.u_Material.ambient = getUniform(objProg, "material.ambient");
	ObjUni.u_Material.diffuse = getUniform(objProg, "material.diffuse");
	ObjUni.u_Material.shininess = getUniform(objProg, "material.shininess");
	ObjUni.u_Material.objColor  = getUniform(objProg, "material.objColor");
	ObjUni.u_Material.spec      = getUniform(objProg, "material.specular");


	LightUni.u_Model = getUniform(lightProg, "u_Model");
	LightUni.u_View = getUniform(lightProg, "u_View");
	LightUni.u_Proj = getUniform(lightProg, "u_Proj");
	LightUni.u_LightCol = getUniform(lightProg, "u_LightCol");

	proj = glm::perspective(glm::radians(c_DefFov), (float)windowWidth / windowHeight, c_NearClip, c_FarClip);

	GL_ERROR(glUseProgram(objProg));
	GL_ERROR(glUniformMatrix4fv(ObjUni.u_Proj, 1, GL_FALSE, &proj[0][0]));
	GL_ERROR(glUniform3f(ObjUni.u_LightCol, lightCol.x, lightCol.y, lightCol.z));
	GL_ERROR(glUniform3f(ObjUni.u_LightPos, lightPos.x, lightPos.y, lightPos.z));

	if (!initTexture(GL_TEXTURE0, "res/Textures/Crate.png")) return false;
	if (!initTexture(GL_TEXTURE1, "res/Textures/Crate-Specular.png")) return false;

	GL_ERROR(glUniform1i(ObjUni.u_Material.objColor, 0));
	GL_ERROR(glUniform1i(ObjUni.u_Material.spec,     1));

	GL_ERROR(glUseProgram(lightProg);)
	GL_ERROR(glUniformMatrix4fv(LightUni.u_Proj, 1, GL_FALSE, &proj[0][0]));
	GL_ERROR(glUniform3f(LightUni.u_LightCol, lightCol.x, lightCol.y, lightCol.z));


	return true;
}

void initImGui() {
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
}

bool initAll(HINSTANCE hInstance, int nCmdShow) {
	initConsole();
	if (!initWindow(hInstance, nCmdShow)) {
		DEBUG_ONLY(printf("Window initialization failed\n"); __debugbreak());
		return false;
	}
	if (!initOpenGL(nCmdShow)) {
		DEBUG_ONLY(printf("OpenGL initialization failed\n"); __debugbreak());
		return false;
	}
	initImGui();

	//Make sure this gets called last in the init
	//Because it will run the windowProc and call events that render, which must be initialized
	ShowWindow(hwnd, nCmdShow);

	return true;
}