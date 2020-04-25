#include "Timer.h"

Timer::Timer() {
	start = std::chrono::high_resolution_clock::now();
	stop = std::chrono::high_resolution_clock::now();
}

void Timer::Restart() {
	isRunning = true;
	start = std::chrono::high_resolution_clock::now();
}

bool Timer::Stop() {
	if (!isRunning) {
		return false;
	} else {
		isRunning = false;
		stop = std::chrono::high_resolution_clock::now();
		return true;
	}
}

bool Timer::Start() {
	if (isRunning) {
		return false;
	} else{
		Restart();
		return true;
	}
}
