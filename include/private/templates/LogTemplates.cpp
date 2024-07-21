#pragma once
#include "Log.h"
#include <sstream>

template <typename Number>
std::string Log::AddCommas(const Number number)
{
	std::stringstream stream;
	stream.imbue(std::locale(""));

	stream << number;
	return stream.str();
}