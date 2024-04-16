#pragma once
#include "GlobalVariables.hpp"

inline bool processEvents() {
	if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) return true;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ULONGLONG currentTime = GetTickCount64();
	float posDiff = c_MoveSensitivy * (currentTime - previousTime);
	if (GetKeyState('W')       & 0x8000) camPos.z += posDiff;
	if (GetKeyState('A')       & 0x8000) camPos.x += posDiff;
	if (GetKeyState('S')       & 0x8000) camPos.z -= posDiff;
	if (GetKeyState('D')       & 0x8000) camPos.x -= posDiff;
	if (GetKeyState(VK_SPACE)  & 0x8000) camPos.y -= posDiff;
	if (GetKeyState(VK_LSHIFT) & 0x8000) camPos.y += posDiff;
	previousTime = currentTime;

	return false;
}

LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
