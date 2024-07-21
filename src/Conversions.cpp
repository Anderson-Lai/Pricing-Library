#include "Conversions.h"
#include <Windows.h>
#include <memory>

std::wstring Conversions::ConvertToWString(const std::string& string)
{
	const int wStringSize = MultiByteToWideChar(CP_UTF8, 0, string.c_str(), -1, nullptr, 0);

	std::unique_ptr<wchar_t[]> result = std::make_unique<wchar_t[]>(wStringSize);
	MultiByteToWideChar(CP_UTF8, 0, string.c_str(), -1, result.get(), wStringSize);

	std::wstring str(result.get());
	return str;
}

std::string Conversions::ConvertToString(const std::wstring& wstring)
{
	const int stringSize = WideCharToMultiByte(CP_UTF8, 0, wstring.c_str(), -1, nullptr, 0, nullptr, nullptr);

	std::string result(stringSize - 1, '\0');
	WideCharToMultiByte(CP_UTF8, 0, wstring.c_str(), -1, &result[0], stringSize, nullptr, nullptr);

	return result;
}