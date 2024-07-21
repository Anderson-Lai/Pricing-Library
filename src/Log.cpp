#include "Log.h"
#include <iostream>

void Log::Error(const std::string& ErrorMessage)
{
	std::cerr << ErrorMessage << "\n";
}

void Log::Println(const std::string& Message)
{
	std::cout << Message << "\n";
}