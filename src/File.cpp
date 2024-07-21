#include "File.hpp"
#include <fstream>
#include "Log.h"
#include "AuctionHouse.hpp"
#include "Conversions.h"
#include "json.hpp"

using json = nlohmann::json;

int File::CreateJson() {
	std::ifstream file("lbin.json");

	if (!file.is_open()) {
		Log::Println("Could not find lbin.json");
		Log::Println("Creating lbin.json");

		std::ofstream createFile("lbin.json");
		if (!createFile.is_open()) {
			Log::Error("Could not create lbin.json");
			return 1;
		}
	}
	else {
		Log::Println("lbin.json was found");
	}

	return 0;
}

json File::ReadData() {
	json data;
	
	std::ifstream file("lbin.json");
	
	if (!file.is_open()) {
		Log::Error("Could not open lbin.json to receive data");
		return data;
	}

	try {
		file >> data;
	}
	catch (const json::parse_error& exception) {
		Log::Error(std::format("Failed to parse json when reading in from file: {}", exception.what()));
	}
	return data;
}

void File::WriteData() {
	json fileData = File::ReadData();

	std::mutex mtx;
	{
		const std::scoped_lock lock(mtx);
		const auto& cachedPrices = AuctionHouse::GetInstance().GetCachedPrices();

		for (const auto& [k, v] : cachedPrices) {
			// prices should already be cleaned from Item::ComputeAverages()
			fileData[Conversions::ConvertToString(k)] = v;
		}
	}

	std::ofstream fileDump("lbin.json");
	if (!fileDump.is_open()) {
		Log::Error("Failed to open lbin.json to write new data");
		return;
	}
	fileDump << fileData.dump(4);
}