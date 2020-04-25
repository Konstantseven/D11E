#pragma once

#include <chrono>

class Timer
{
public:
	Timer();

	inline double GetMilisecondsElapsed() const {
		return std::chrono::duration<double, std::milli>((isRunning ? std::chrono::high_resolution_clock::now() : stop) - start).count();
	}

	bool Stop();
	bool Start();
	void Restart();

private:
	bool isRunning = false;

	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::time_point<std::chrono::steady_clock> stop;
};
