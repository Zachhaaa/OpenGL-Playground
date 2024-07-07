#pragma once

#include <Windows.h>
#include <ShellScalingApi.h>
#include <glad/glad.h>
#include <OpenGL-Manager.hpp>

typedef BOOL(WINAPI* WGLSWAPINTERVALEXT)(int);
extern BOOL(WINAPI* wglSwapIntervalEXT)(int);

/// mam is short for manager
namespace man {

	class Window {
		HWND hwnd;
		HDC dc;
		HGLRC glrc;
		int nCmdShow;
		bool windowStatus = true;
	public:
		/**
		* Creates the Window for OpenGL
		* 
		* @param hInstance use from WinMain parameter
		* @param nCmdShow use from WinMain parameter
		* @param winProc write your own winProc function to manage events
		*/
		Window(HINSTANCE hInstance, int nCmdShow, int windowWidth, int windowHeight, const wchar_t* windowTitle, WNDPROC winProc)
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

			float scale = dpiX / 96.0;  
			hwnd = CreateWindowEx(
				0,
				windowTitle,
				windowTitle,
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				int(windowWidth * scale),
				int(windowHeight * scale),
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
			pFormat = ChoosePixelFormat(dc, &pfd);
			if (!pFormat) {
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

			wglSwapIntervalEXT(-1);

			GL_ERROR(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
			GL_ERROR(glEnable(GL_DEPTH_TEST));
			GL_ERROR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		}
		~Window() {
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(glrc);
			ReleaseDC(hwnd, dc);
		}
		bool getWindowStatus() { return windowStatus; }
	};
}