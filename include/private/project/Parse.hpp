#pragma once

#include <vector>
#include <string>

#include "Item.hpp"
#include "json.hpp"

using json = nlohmann::json;

namespace Parse {
	json ConvertToJson(const std::string& data);
	std::vector<Item> Parse(std::vector<Item>& data);
}