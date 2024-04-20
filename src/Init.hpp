#pragma once

#include <Windows.h>
#include <ShellScalingApi.h>

#include <cstdio>

#include "Events.hpp"

typedef BOOL (WINAPI* WGLSWAPINTERVALEXT)(int);
extern BOOL (WINAPI* wglSwapIntervalEXT)(int);

void initConsole();
float getInitMonitorScale();
bool initWindow(HINSTANCE hInstance, int nCmdShow);
bool initOpenGL(int nCmdShow);
bool initAll(HINSTANCE hInstance, int nCmdShow);

