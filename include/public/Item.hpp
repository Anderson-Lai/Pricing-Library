#pragma once

#include <string>
#include <array>

class Item {
public:
	Item(const std::string& playerId, const std::wstring& itemName, const std::wstring& rarity, const std::wstring& itemLore,
		const std::string& category, const long long price);
	
	void CalculateProfit();

	static void CleanName(
		std::wstring& name, const std::string& category, std::wstring& rarity, const std::wstring& itemLore);

public:
	std::string auctionId;
	std::wstring itemName;
	std::string category; // weapon, armor (no 'u'), misc, etc.
	std::wstring rarity;
	std::wstring itemLore;
	long long price;
	long long profit;
private:
	static bool isAlphaNumeric(const wchar_t letter);
	static bool isDuplicatePrefix(const std::wstring& prefix);
	static bool isReforgePrefix(const std::wstring& prefix);
	static bool isRecombobulated(const std::wstring& itemLore);
private:
	static const std::array<wchar_t, 68> s_alphaNumeric;
	static const std::array<std::wstring, 10> s_duplicatePrefixes;
	static const std::array<std::wstring, 114> s_reforgePrefixes;
	static const std::array<std::wstring, 8> s_rarities;
};

inline const std::array<wchar_t, 68> Item::s_alphaNumeric = { L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9',
L'a', L'b', L'c', L'd', L'e', L'f', L'g', L'h', L'i', L'j', L'k', L'l', L'm', L'n', L'o', L'p', L'q', L'r', L's', L't', L'u', L'v',
L'w', L'x', L'y', L'z', L'A', L'B', L'C', L'D', L'E', L'F', L'G', L'H', L'I', L'J', L'K', L'L', L'M', L'N', L'O', L'P' , L'Q',
L'R', L'S', L'T', L'U', L'V', L'W', L'X', L'Y', L'Z', L'-', L'\'', L'™', L'©', L',', L'⚚' };

inline const std::array<std::wstring, 10> Item::s_duplicatePrefixes = {
	L"Very", L"Highly", L"Extremely", L"Not", L"So", L"Thicc",
	L"Absolutely", L"Even", L"More", L"Greater"
};

inline const std::array<std::wstring, 114> Item::s_reforgePrefixes = {
	// sword reforges (19 total)
	L"Epic", L"Fair", L"Fast", L"Gentle", L"Heroic", L"Legendary", L"Odd", L"Sharp", L"Spicy", L"Coldfused",
	L"Dirty", L"Fabled", L"Gilded", L"Suspicious", L"Warped", L"Withered", L"Bulky", L"Jerry's", L"Fanged",

	// bow reforges (12 total)
	L"Awkward", L"Deadly", L"Fine", L"Grand", L"Hasty", L"Neat", L"Rapid", L"Rich", L"Unreal", L"Precise",
	L"Spiritual", L"Headstrong",

	// armour reforges (28 total)
	L"Clean", L"Fierce", L"Heavy", L"Light", L"Mythic", L"Pure", L"Titanic", L"Smart", L"Wise", L"Candied",
	L"Submerged", L"Perfect", L"Reinforced", L"Renowned", L"Spiked", L"Hyper", L"Giant", L"Jaded", L"Cubic", L"Necrotic",
	L"Empowered", L"Ancient", L"Undead", L"Loving", L"Ridiculous", L"Bustling", L"Mossy", L"Festive", 

	// equipment reforges (17 total)
	L"Stained", L"Menacing", L"Hefty", L"Soft", L"Honored", L"Blended", L"Astute", L"Colossal", L"Brilliant", L"Glistening",
	L"Strengthened", L"Waxed", L"Fortified", L"Rooted", L"Blooming", L"Snowy", L"Blood-Soaked",

	// fishing rod reforges (7 total)
	L"Salty", L"Treacherous", L"Lucky", L"Stiff", L"Dirty", L"Chomp", L"Pitchin'",

	// pickaxe reforges (14 total)
	L"Unyielding", L"Prospector's", L"Excellent", L"Sturdy", L"Fortunate", L"Ambered", L"Auspicious", L"Fleet", L"Heated", L"Magnetic",
	L"Mithraic", L"Refined", L"Stellar", L"Fruitful",

	// axe reforges (9 total)
	L"Great", L"Rugged", L"Lush", L"Lumberjack's", L"Double-Bit", L"Moil", L"Toil", L"Blessed", L"Earthy",

	// hoe reforges (6 total)
	L"Robust", L"Zooming", L"Peasant's", L"Green Thumb", L"Blessed", L"Bountiful", 

	// vacuum reforges (2 total) (what is a vacuum???)
	L"Beady", L"Buzzing",
};

inline const std::array<std::wstring, 8> Item::s_rarities = { L"COMMON", L"UNCOMMON", L"RARE", L"EPIC", 
L"LEGENDARY", L"MYTHIC", L"SPECIAL", L"VERY_SPECIAL"};