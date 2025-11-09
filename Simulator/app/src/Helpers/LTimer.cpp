#include "../../incl/LTimer.h"

typedef std::chrono::high_resolution_clock Clock;
LTimer::LTimer() {
	// Initialize the timer
}

void LTimer::start() {
	bIsRunning = true;
	startTime = Clock::now();
}

std::chrono::milliseconds LTimer::getTicks() {
	curTime = Clock::now();
	auto t = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - startTime);
	return t;
}

bool LTimer::timerHasTicked(float interval) {
	std::chrono::milliseconds t = getTicks();
	if ((float)(t.count()) > interval) {
		startTime = curTime;
		return true;
	}
	else {
		return false;
	}
}

void LTimer::stop() {
	bIsRunning = false;
}

void LTimer::reset() {
	bIsRunning = true;
	startTime = Clock::now();
}

bool LTimer::isRunning() const {
	return bIsRunning;
}