#include "GlobalVariables.hpp"

unsigned
windowWidth,
windowHeight,
windowMinWidth,
windowMinHeight;
float scale;

// Scene 
pug::vec3f lightCol = { 1.0f,  1.0f,  1.0f };
pug::vec3f objCol = { 0.95f, 0.42f, 0.21f };
pug::vec3f objAmbient = 0.35f * objCol;
pug::vec3f objDiffuse = objCol;
pug::vec3f objSpecular = 0.6 * objCol;
float      objShininess = 32.0f;
glm::vec3  lightPos(2.0f, 3.0f, 4.0f);

// General global variables:
ULONGLONG appStartTime;
ULONGLONG previousTime;
glm::mat4 model, view, proj;

// Camera
glm::vec3 camPos(0.0f, 0.0f, -3.0f);
pug::vec2f camAng = { 0.0f, 0.0f };


// win32 global variables:
HWND hwnd;
HDC dc;
HGLRC glrc;
HANDLE consoleHandle;
MSG msg = {};
bool windowFocus = true;

// OpenGL global variables"
GLuint objProg, lightProg;
App::ObjUniforms ObjUni;
App::LightUniforms LightUni;