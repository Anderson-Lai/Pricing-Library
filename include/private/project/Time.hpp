#pragma once

#include <chrono>

namespace Time {
	std::chrono::time_point<std::chrono::steady_clock> Now();
	long long SecondsSinceEpoch();
	void Print(std::chrono::time_point<std::chrono::steady_clock> start, 
		std::chrono::time_point<std::chrono::steady_clock> end);
	void Sleep(unsigned long long milliseconds);
}