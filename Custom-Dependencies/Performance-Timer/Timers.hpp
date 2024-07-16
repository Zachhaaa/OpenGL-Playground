#pragma once

#include <Performance-Timer.hpp>

struct Timers {
	double cutoffAngleChange = -1.0;
	double cutoffAngleCopy = -1.0;
	double cutoffAngleSmooth = -1.0;
	double cutoffAngleSubData = -1.0;
	double clearBufferTime = -1.0;
	Timers() {
		QueryPerformanceFrequency(&timerFrequency);
	}
};
extern Timers timers;