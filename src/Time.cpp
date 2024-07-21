#include "Time.hpp"
#include <format>
#include <chrono>
#include <thread>
#include "Log.h"

std::chrono::time_point<std::chrono::steady_clock> Time::Now() {
	return std::chrono::high_resolution_clock::now();
}

long long Time::SecondsSinceEpoch() {
	const auto now = std::chrono::system_clock::now();
	auto sinceEpoch = now.time_since_epoch();
	const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(sinceEpoch).count();
	return seconds;
}

void Time::Print(std::chrono::time_point<std::chrono::steady_clock> start, 
	std::chrono::time_point<std::chrono::steady_clock> end) {
	
	auto timeElapsed = std::chrono::duration<double, std::milli>(end - start);

	Log::Println(std::format("Time taken: {}", timeElapsed));
}

void Time::Sleep(unsigned long long milliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}