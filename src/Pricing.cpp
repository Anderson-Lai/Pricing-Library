#include "Pricing.h"
#include <thread>
#include "AuctionHouse.hpp"
#include "Time.hpp"
#include "Log.h"
#include "curl/curl.h"
#include "File.hpp"
#include "Conversions.h"
#include "Clipboard.h"
#include "Parse.hpp"

static void ReadFileData();
static void UpdateFile();
static void UpdateLbins();

static Api getFirstPage;

void Pricing::Start()
{
	curl_global_init(CURL_GLOBAL_DEFAULT);

	if (File::CreateJson())
	{
		Log::Error("Program can not continue without lbin.json");
		Log::Error("Exiting program");
		return;
	}

	ReadFileData();

	// update lbin.json
	std::thread updateFile(UpdateFile);
	
	// update in-memory lbins
	std::thread updateLbins(UpdateLbins);

	updateFile.detach();
	updateLbins.detach();

	// allow for keep-alive probes to be sent
	getFirstPage.CallApi("https://api.hypixel.net/v2/skyblock/auctions?page=0");

	Log::Println("\nInitialization complete");
}

std::vector<Item> Pricing::GetFlips()
{
	const auto start = Time::Now();

	const auto scrapeStart = Time::Now();
	std::vector<Item> recents = AuctionHouse::ScrapeNewBins(getFirstPage);
	const auto scrapeEnd = Time::Now();

	const auto parseStart = Time::Now();
	std::vector<Item> returned = Parse::Parse(recents);
	const auto parseEnd = Time::Now();

	// copy the highest profit-to-ratio item's auction id to clipboard
	if (!returned.empty())
	{
		Clipboard::WriteToClipboard("/viewauction " + returned.back().auctionId);
	}

	const auto end = Time::Now();

	Log::Println("\nScraping items:");
	Time::Print(scrapeStart, scrapeEnd);

	Log::Println("\nParsing items:");
	Time::Print(parseStart, parseEnd);

	Log::Println("\nTotal time for everything:");
	Time::Print(start, end);

	return returned;
}

static void ReadFileData()
{
	AuctionHouse& instance = AuctionHouse::GetInstance();

	Log::Println("\nReading from file!");
	const auto readStart = Time::Now();
	instance.SeedFromFileData();
	const auto readEnd = Time::Now();

	Time::Print(readStart, readEnd);

	Log::Println("\nScraping all bins!");

	const auto scrapeStart = Time::Now();
	instance.ScrapeAllBins();
	const auto scrapeEnd = Time::Now();

	Time::Print(scrapeStart, scrapeEnd);
}

static void UpdateFile()
{
	while (true)
	{
		Time::Sleep(60'000);

		Log::Println("\nWriting to file!");

		const auto fileStart = Time::Now();
		File::WriteData();
		const auto fileEnd = Time::Now();

		Time::Print(fileStart, fileEnd);
	}
}

static void UpdateLbins()
{
	while (true)
	{
		const long long sinceEpoch = Time::SecondsSinceEpoch();
		const long long secondsToMinute = sinceEpoch % 60;
		if (secondsToMinute == 5)
		{
			Time::Sleep(60'000);
			AuctionHouse& instance = AuctionHouse::GetInstance();

			Log::Println("\nScraping all bins!");

			const auto scrapeStart = Time::Now();
			instance.ScrapeAllBins();
			const auto scrapeEnd = Time::Now();

			Time::Print(scrapeStart, scrapeEnd);
		}
		else
		{
			Time::Sleep(1'000);
		}
	}
}
