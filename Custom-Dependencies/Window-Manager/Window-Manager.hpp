#pragma once

#include <Windows.h>
#include <ShellScalingApi.h>
#include <glad/glad.h>
#include <OpenGL-Manager.hpp>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_opengl3.h>

typedef BOOL(WINAPI* WGLSWAPINTERVALEXT)(int);

/// Man is short for manager
namespace Man {

	class Window {
		HWND hwnd;
		HDC dc;
		HGLRC glrc;
		int nCmdShow;
		int windowWidth;
		int windowHeight; 
		bool windowStatus = true;
		BOOL(WINAPI* wglSwapIntervalEXT)(int);

	public:
		/**
		* Creates the Window for OpenGL
		* 
		* @param hInstance use from WinMain parameter
		* @param nCmdShow use from WinMain parameter
		* @param winProc write your own winProc function to manage events
		*/
		Window(HINSTANCE hInstance, int nCmdShow, const wchar_t* windowTitle, WNDPROC winProc, float aspectRatio, float percentSize);

		void show() { ShowWindow(hwnd, nCmdShow); }
		/// @return true = success, false = fail
		bool getWindowStatus() { return windowStatus; }
		void swapBuffers() { SwapBuffers(dc); }
		void clearBuffer() { GL_ERROR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)); }
		void setSwapInterval(int a) { wglSwapIntervalEXT(a); }
		HWND getHwnd() { return hwnd; }
		int  getWindowWidth() { return windowWidth; }
		int  getWindowHeight() { return windowHeight; }

		~Window() {
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(glrc);
			ReleaseDC(hwnd, dc);
			DestroyWindow(hwnd);
		}
	};
}