#ifndef UNICODE
#define UNICODE
#endif

#include "Shader-Programs.hpp"
#include <Window-Manager.hpp>

#include "Constants.hpp"

bool quit = false;

LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
		quit = true;
        return 0;
	case WM_QUIT:
		quit = true;
		return 0;
	case WM_CLOSE: 
		quit = true;
		return 0;
    }


    return DefWindowProc(hwnd, msg, wParam, lParam);
}
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
GLint attribSizes[] = {3, 3, 2};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

	Man::Window window(hInstance, nCmdShow, c_WindowWidth, c_WindowHeight, L"Opengl Sandbox", winProc);
	if(!window.getWindowStatus()) return 1;
	Man::Geometry geometry(vertices, attribSizes, sizeof(attribSizes) / sizeof(GLint), 36);
	ObjectShader objShdr;
	if (!objShdr.getShaderStatus()) { return 2; }
	LightShader lghtShdr;
	if (!lghtShdr.getShaderStatus()) { return 3; }

	objShdr.bind();

	glm::mat4 model = glm::mat4(1.0f);
	objShdr.model(model); 
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
	objShdr.view(view);
	glm::mat4 proj = glm::perspective(
		glm::radians(c_DefaultFOV), 
		(float)c_WindowWidth / c_WindowHeight, 
		c_NearClippingDistance, 
		c_FarClippingDistance
	);
	objShdr.proj(proj);

	objShdr.lightCol(glm::vec3(1.0f, 1.0f, 1.0f));
	objShdr.lightPos(glm::vec3(2.0f, 2.0f, 3.0f));
	// NOTE: This and the player position need to say in sync.
	objShdr.viewPos(glm::vec3(0.0f, 0.0f, -3.0f));

	objShdr.materialAmbient(0.3f);
	objShdr.materialDiffuse(0.3f);
	objShdr.materialShininess(0.3f);


	lghtShdr.bind(); 

	lghtShdr.model(model);
	lghtShdr.view(view);
	lghtShdr.proj(proj); 
	// NOTE: This and the light color need to sync between shaders.
	lghtShdr.lightCol(glm::vec3(1.0f, 1.0f, 1.0f)); 

	MSG msg = {};
    while (true) {
        if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if(quit) break;
        }
		objShdr.bind();
		geometry.render(); 

		lghtShdr.bind();
		geometry.render();


        window.swapBuffers();
        window.clearBuffer();
    }


	return 0;  
}
