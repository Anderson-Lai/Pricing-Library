#pragma once

#include <string>

namespace Conversions
{
	std::wstring ConvertToWString(const std::string& string);
	std::string ConvertToString(const std::wstring& wstring);
}