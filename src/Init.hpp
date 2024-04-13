#pragma once

#include <Windows.h>
#include <ShellScalingApi.h>

#include <cstdio>



#include "Events.hpp"

void initConsole();
float getInitMonitorScale();
bool initWindow(HINSTANCE hInstance, int nCmdShow);
bool initOpenGL(int nCmdShow);
bool initAll(HINSTANCE hInstance, int nCmdShow);

