#include "Timer.h"

Timer::Timer() {
	start = std::chrono::high_resolution_clock::now();
	stop = std::chrono::high_resolution_clock::now();
}

void Timer::Restart() {
	isRunning = true;

	start = std::chrono::high_resolution_clock::now();
	stop = std::chrono::high_resolution_clock::now();
}

bool Timer::Stop() {
	if (isRunning) {
		isRunning = false;
		stop = std::chrono::high_resolution_clock::now();

		return true;
	}

	return false;
}

bool Timer::Start() {
	if (!isRunning) {
		Restart();

		return true;
	}

	return false;
}
