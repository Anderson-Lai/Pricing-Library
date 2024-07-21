#pragma once

#include "json.hpp"

using json = nlohmann::json;

namespace File {
	int CreateJson();
	json ReadData();
	void WriteData();
}