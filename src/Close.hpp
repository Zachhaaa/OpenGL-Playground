#pragma once

#include "GlobalVariables.hpp"

inline void closeApp() {
	FreeConsole();
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(glrc);
	ReleaseDC(hwnd, dc);
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}