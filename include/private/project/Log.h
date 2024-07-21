#pragma once

#include <string>

namespace Log
{
	void Println(const std::string& message = "");
	void Error(const std::string& error);
	
	template<typename Number>
	std::string AddCommas(const Number number);
}

#include "LogTemplates.cpp"