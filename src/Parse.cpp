#include "Parse.hpp"
#include <ranges>
#include "Log.h"

using json = nlohmann::json;

json Parse::ConvertToJson(const std::string& data) {
	json result;

	try {
		result = json::parse(data);
	}
	catch (const json::parse_error& exception) {
		Log::Error(std::format("Could not parse json correctly: {}", exception.what()));
	}
	return result;
}

std::vector<Item> Parse::Parse(std::vector<Item>& data) {
	std::vector<Item> profitable;

	for (auto& bin : data) {
		Item::CleanName(bin.itemName, bin.category, bin.rarity, bin.itemLore);
		bin.CalculateProfit();

		if (bin.profit >= 1'000'000) {
			profitable.emplace_back(std::move(bin));
		}
	}

	std::ranges::sort(profitable, [](const Item& first, const Item& second) {
		return first.profit < second.profit;
		});

	return profitable;
}