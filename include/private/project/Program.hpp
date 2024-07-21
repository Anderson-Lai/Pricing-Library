#pragma once

#include <string>

class Program {
public:
	void Run();
	static Program& GetInstance();
private:
	static void ReadFileData();
	static void UpdateFile();
	static void UpdateLbins();
	static void WriteToClipboard(const std::string& text);

	// singleton design for the Program object
	Program();
	~Program();
	
	// delete move and copy constructor, along with the = operator
	Program(const Program& copy) = delete;
	Program(Program&& move) = delete;
	Program& operator=(const Program& copy) = delete;
	Program& operator=(Program&& move) = delete;
};
