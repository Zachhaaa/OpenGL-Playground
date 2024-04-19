#pragma once

#include "GlobalVariables.hpp"

extern POINT prevMousePos;
extern pug::vec3f keyVec;

inline bool processEvents() {
	if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) return true;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (!windowFocus) return false;

	/*POINT curMousePos;
	GetCursorPos(&curMousePos); 
	if (GetAsyncKeyState(VK_MBUTTON) & 0x8000) {
		camAng.y += c_MouseSensitivity * (curMousePos.x - prevMousePos.x);
		camAng.x += c_MouseSensitivity * (curMousePos.y - prevMousePos.y);
	}
	prevMousePos.x = curMousePos.x;
	prevMousePos.y = curMousePos.y;*/

	ULONGLONG currentTime = GetTickCount64();
	float posDiff = c_MoveSensitivity * (currentTime - previousTime);
	if (GetAsyncKeyState('W') & 0x8000) camPos.z += posDiff;
	if (GetAsyncKeyState('A') & 0x8000) camPos.x += posDiff;
	if (GetAsyncKeyState('S') & 0x8000) camPos.z -= posDiff;
	if (GetAsyncKeyState('D') & 0x8000) camPos.x -= posDiff;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) camPos.y -= posDiff;
	if (GetAsyncKeyState(VK_LSHIFT) & 0x8000) camPos.y += posDiff;
	previousTime = currentTime;

	/*ULONGLONG currentTime = GetTickCount64();
	float posDiff = c_MoveSensitivity * (currentTime - previousTime);
	camPos.x += posDiff * keyVec.x;
	camPos.y += posDiff * keyVec.y;
	camPos.z += posDiff * keyVec.z;
	previousTime = currentTime;*/

	return false;
}

LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
