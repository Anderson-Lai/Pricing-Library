#pragma once

#include <shared_mutex>
#include <unordered_map>
#include <queue>
#include <vector>

#include "Item.hpp"
#include "Api.hpp"
#include "json.hpp"

using json = nlohmann::json;

class AuctionHouse {
public:
	static AuctionHouse& GetInstance();
	long long LookupPrice(const std::wstring& itemName) const;
	void ScrapeAllBins();
	const std::unordered_map<std::wstring, long long>& GetCachedPrices() const;
	void SeedFromFileData();
	static std::vector<Item> ScrapeNewBins(Api& getFirstPage);
private:
	std::unordered_map<std::wstring, 
		std::priority_queue<long long, std::vector<long long>, std::greater<long long>>> m_averages;

	std::unordered_map<std::wstring, long long> m_prices;
	mutable std::shared_mutex m_mtx;
private:
	void ComputeAverages();
	void ProcessData(const json& data);

	AuctionHouse() = default;
	~AuctionHouse() = default;
	AuctionHouse(const AuctionHouse& other) = delete;
	AuctionHouse(AuctionHouse&& other) = delete;

	AuctionHouse& operator=(const AuctionHouse& other) = delete;
	AuctionHouse& operator=(AuctionHouse&& other) = delete;
};