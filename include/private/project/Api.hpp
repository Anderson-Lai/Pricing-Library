#pragma once

#include <string>

#include "curl/curl.h"

class Api {
public:
	const std::string& CallApi(const char* url);
	Api();
	~Api();
private:
	CURL* m_curl = nullptr;
	CURLcode m_result;
	std::string m_buffer;
private:
	static size_t WriteData(void* responseContents, size_t elementSize, size_t numberOfElements, void* userPointer);

	// prevents copying
	Api(const Api& CopyConstructor) = delete;
	Api(Api&& MoveConstructor) = delete;
	Api& operator=(const Api& copy) = delete;
	Api& operator=(Api&& move) = delete;
};