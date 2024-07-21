#include "Program.hpp"
#include <Windows.h>
#include "Log.h"
#include "curl/curl.h"

Program& Program::GetInstance()
{
	static Program instance;
	return instance;
}

void Program::WriteToClipboard(const std::string& text)
{
	if (!OpenClipboard(nullptr))
	{
		Log::Error("Could not open clipboard");
		return;
	}
	EmptyClipboard();

	size_t bufferSize = text.size() + 1;

	// malloc global memory (memory that can be accessed by any program) to hold clipboard text
	HGLOBAL clipboardMemory = GlobalAlloc(GMEM_MOVEABLE, (bufferSize) * sizeof(char));
	if (clipboardMemory == nullptr)
	{
		CloseClipboard();
		return;
	}

	// type cast the global memory to a char* pointer
	// lock the memory since ALL programs can access this memory
	char* clipboardText = (char*)GlobalLock(clipboardMemory);

	// checks if the lock failed
	if (!clipboardText)
	{
		GlobalFree(clipboardMemory);
		CloseClipboard();
		return;
	}

	// copy text to buffer
	if (strcpy_s(clipboardText, (bufferSize), text.c_str()) != 0)
	{
		Log::Error("Error copying text to clipboardText buffer");
		GlobalFree(clipboardText);
		CloseClipboard();
		return;
	}

	GlobalUnlock(clipboardMemory);
	// ownership of clipboardMemory is transferred
	SetClipboardData(CF_TEXT, clipboardText);

	// clean up
	CloseClipboard();
}

Program::Program()
{
	// initialize curl once when program starts
	curl_global_init(CURL_GLOBAL_DEFAULT);
}

Program::~Program()
{
	// cleanup at end of program automatically
	curl_global_cleanup();
}