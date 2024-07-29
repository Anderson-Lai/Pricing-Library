#include "AuctionHouse.hpp"
#include <format>
#include "Api.hpp"
#include "Parse.hpp"
#include "Time.hpp"
#include "File.hpp"
#include "Conversions.h"
#include "Log.h"
#include <mutex>

using json = nlohmann::json;

AuctionHouse& AuctionHouse::GetInstance() {
	static AuctionHouse instance;
	return instance;
}

long long AuctionHouse::LookupPrice(const std::wstring& itemName) const {
	const std::shared_lock lock(m_mtx);

	if (m_prices.find(itemName) == m_prices.end()) {
		return INT_MIN;
	}
	else {
		return m_prices.at(itemName);
	}
}

void AuctionHouse::ScrapeAllBins() {

	{
		const std::unique_lock lock(m_mtx);
		this->m_averages.clear();
	}

	Api getFirstPage;

	const std::string& response = getFirstPage.CallApi("https://api.hypixel.net/v2/skyblock/auctions?page=0");
	json converted = Parse::ConvertToJson(response);

	if (converted.is_null()) {
		Log::Error("Json parsing error occured while scraping first page of auction to update lbin data");
		return;
	}

	// number of pages of auction data
	const int pages = converted["totalPages"];

	// process the first page
	this->ProcessData(converted);

	// process the other pages
	for (int i = 1; i < pages; i++) {
		Api getPage;

		const std::string& response = 
			getPage.CallApi((std::format("https://api.hypixel.net/v2/skyblock/auctions?page={}", i)).c_str());
		json converted = Parse::ConvertToJson(response);

		if (converted.is_null()) {
			Log::Error(std::format(
				"Json parsing error occurred while scraping page {} of the auction house to update lbin data", i));
			continue;
		}

		this->ProcessData(converted);
	}

	this->ComputeAverages();
}

void AuctionHouse::ProcessData(const json& data)
{
	// lowers contention for the mutex
	const std::unique_lock lock(m_mtx);
	// iterate through all the auctions in the called page
	for (const auto& auction : data["auctions"])
	{
		if (!auction["bin"])
		{
			continue;
		}

		// clean the item names
		std::wstring name = Conversions::ConvertToWString(auction["item_name"]);
		std::wstring rarity = Conversions::ConvertToWString(auction["tier"]);
		Item::CleanName(name, auction["category"], rarity, Conversions::ConvertToWString(auction["item_lore"]));

		// save it into averages hashmap (thread safe due to m_mtx)
		m_averages[name].push(auction["starting_bid"]);
	}
}

const std::unordered_map<std::wstring, long long>& AuctionHouse::GetCachedPrices() const {
	std::shared_lock lock(m_mtx);
	return this->m_prices;
}

void AuctionHouse::SeedFromFileData() {
	const std::unique_lock lock(m_mtx);
	this->m_prices.clear();

	json fileData = File::ReadData();

	if (fileData.is_null()) {
		return;
	}

	for (auto it = fileData.begin(); it != fileData.end(); it++) {
		m_prices[Conversions::ConvertToWString(it.key())] = it.value().get<long long>();
	}
}

std::vector<Item> AuctionHouse::ScrapeNewBins(Api& getFirstPage) {

	const std::string& response = getFirstPage.CallApi("https://api.hypixel.net/v2/skyblock/auctions?page=0");
	json converted = Parse::ConvertToJson(response);

	std::vector<Item> items;
	if (converted.is_null()) {
		Log::Error("Json parsing error occurred while scraping newly created bins");
		return items;
	}

	items.reserve(converted["auctions"].size());

	long long now = Time::SecondsSinceEpoch();
	now *= 1'000; // convert to milliseconds

	for (const auto& auction : converted["auctions"]) {
		if (!auction["bin"] || now - auction["start"].get<long long>() > 62'500) {
			continue; // not recent or not bin -> useless
		}
		items.emplace_back(
			auction["uuid"], Conversions::ConvertToWString(auction["item_name"]), 
			Conversions::ConvertToWString(auction["tier"]), Conversions::ConvertToWString(auction["item_lore"]),
			auction["category"], auction["starting_bid"]
		);
	}

	return items;
}

void AuctionHouse::ComputeAverages() {
	const std::unique_lock lock(m_mtx);

	for (auto& [k, v] : m_averages) { // should be (name, pq of 15+ elements)
		if (v.size() < 15) {
			// too little items -> could be being manipulated
			continue;
		}

		const int pricesToAverage = 5;
		unsigned long long sum = 0;
		int counter = 0;
		long long secondLowest = 0;

		// remove the lowest price, as that could be "profitable", and thus ruin the data
		v.pop(); 
		while (!v.empty() && counter < pricesToAverage) {
			if (counter == 0) {
				secondLowest = v.top();
			}
			sum += v.top();
			v.pop();
			counter++;
		}

		/*
		* Assume the initial scrape of entire ah has no faulty data 
		* (this assumption can be dropped once writing to json has been added)
		* percentageDifference is attempting to mitigate it for now (will still be here even with json)
		* 
		* (this is in case someone say, lists a hyperion for 10 coins
		* the percentageDifference will then be through the roof, somewhere in the orders of > 10
		* thus, the value stored in the hashmap will be the average, due to the ternary expression)
		* 
		* new bins listed will be found by AuctionHouse::ScrapeNewBins()
		* then the average lbin price stored in m_prices will be searched up
		* however, m_prices is "lagging behind" by one auction update
		* recall that the hypixel api updates every minute
		* the std::jthread always gives "priority" to the main while (true) loop by sleeping
		* 
		* however, a possible edge case arises for the first call
		* due to the priority the main loop has, say the jthread immediately sleeps on program start
		* ScrapeNewBins is called, and no profits will be found due to m_prices being empty
		* 
		* if this new ah "push" / api update contains faulty data, the m_prices hashmap will be ruined
		* however, this circles back to the initial assumption, which covers this edge case
		* (this will be fixed by having a json file store past average lbins)
		*/

		const unsigned long long average = sum / pricesToAverage;
		const double percentageDifference = (average - secondLowest) / (double)secondLowest;
		const long long newLbin = percentageDifference <= 0.20 ? secondLowest : average;

		if (m_prices.find(k) == m_prices.end()) {
			m_prices[k] = newLbin;
		}
		else {
			const long long cachedPrice = m_prices[k];

			// data should be clean "enough" at this point
			if (newLbin < cachedPrice) {
				m_prices[k] = newLbin;
			}
			else {
				const double percentage = (newLbin - cachedPrice) / (double)cachedPrice;
				m_prices[k] = percentage <= 0.25 ? newLbin : cachedPrice;
			}
		}
	}
}