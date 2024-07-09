#pragma once

//#include "GlobalVariables.hpp"

extern POINT prevMousePos;
extern pug::vec3f playerVec;
extern float sina, cosa; 

inline bool processEvents() {
	if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (quit) 
			return true;
	}
	if(isMinimized) return false;

	POINT curMousePos;
	GetCursorPos(&curMousePos);
	if (GetAsyncKeyState(VK_MBUTTON) & 0x8000) {
		camAng.y += c_MouseSensitivity * (curMousePos.x - prevMousePos.x);
		camAng.x += c_MouseSensitivity * (curMousePos.y - prevMousePos.y);
		if      (camAng.x > glm::radians(90.0f))  camAng.x = glm::radians(90.0f);
		else if (camAng.x < glm::radians(-90.0f)) camAng.x = glm::radians(-90.0f);
		sina = sin(camAng.y);
		cosa = cos(camAng.y);
	}
	prevMousePos = curMousePos;

	if (!windowFocus) return false;

	playerVec = {0.0f, 0.0f, 0.0f};
	if (GetAsyncKeyState('W') & 0x8000)       playerVec.z +=  1.0f;

	if (GetAsyncKeyState('A') & 0x8000)       playerVec.x += -1.0f;
	if (GetAsyncKeyState('S') & 0x8000)       playerVec.z += -1.0f;
	if (GetAsyncKeyState('D') & 0x8000)       playerVec.x +=  1.0f;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)  playerVec.y += -1.0f;
	if (GetAsyncKeyState(VK_LSHIFT) & 0x8000) playerVec.y +=  1.0f;

	ULONGLONG currentTime = GetTickCount64();
	float posDiff = c_MoveSensitivity * (currentTime - previousTime);
	camPos.x +=  posDiff * -(cosa * playerVec.x + sina * playerVec.z);
	camPos.y +=  posDiff * playerVec.y;
	camPos.z +=  posDiff *  (cosa * playerVec.z - sina * playerVec.x);
	previousTime = currentTime;

	return false;
}

LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
