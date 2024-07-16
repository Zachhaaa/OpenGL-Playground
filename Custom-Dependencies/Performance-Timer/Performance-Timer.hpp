#pragma once

#include <Windows.h>

extern LARGE_INTEGER timerFrequency;

class PerfTimer {
	LARGE_INTEGER start;
public: 
	/// MUST set timerFrequency global variable using QueryPerformanceFrequency() before for the first time.
	/// This only needs to be done once. 
	PerfTimer() { QueryPerformanceCounter(&start); }
	double elapsed() {
		LARGE_INTEGER end;
		QueryPerformanceCounter(&end); 
		return 1000.0 * (end.QuadPart - start.QuadPart) / timerFrequency.QuadPart;
	}
};