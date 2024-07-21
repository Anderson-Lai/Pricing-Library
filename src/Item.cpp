#include "Item.hpp"
#include <sstream>
#include <ranges>
#include <algorithm>
#include <string>
#include "AuctionHouse.hpp"


Item::Item(const std::string& auctionId, const std::wstring& itemName, const std::wstring& rarity,
	const std::wstring& itemLore, const std::string& category, const long long price) :
	auctionId(auctionId), itemName(itemName), rarity(rarity), itemLore(itemLore),
	category(category), price(price), profit(-1) {};

void Item::CleanName(
	std::wstring& name, const std::string& category, std::wstring& rarity, const std::wstring& itemLore) {

	if (Item::isRecombobulated(itemLore)) {
		size_t index = std::ranges::find(s_rarities, rarity) - s_rarities.begin();
		if (index > 0) {
			index--;
		}
		rarity = s_rarities.at(index);
	}

	if (category == "armor" || category == "weapon" || category == "misc") {
		// clean non alpha numeric words (dungeon stars and pet lvls)
		std::wstringstream splitter(name);
		name.clear();

		std::wstring temp = L"";
		while (splitter >> temp) {
			bool useful = true;
			for (size_t i = 0; i < temp.size(); i++) {
				if (!Item::isAlphaNumeric(temp[i])) {
					useful = false;
					break;
				}
			}
			if (!useful) {
				continue;
			}
			name += (temp + L" ");
		}

		while (!name.empty() && name.back() == ' ') {
			name.pop_back();
		}

		// removing reforge
		// allows for reusing
		temp.clear();
		splitter.clear();

		splitter.str(name);
		name.clear();

		// first, consider the duplicate prefixes
		std::wstring next = L"";
		while (splitter >> temp) {
			if (Item::isDuplicatePrefix(temp)) {
				continue;
			}

			// all the edge cases
			if (temp == L"Wise" || temp == L"Strong" || temp == L"Superior") {
				splitter >> next;
				if (next == L"Dragon") {
					name += (temp + L" " + next + L" ");
				}
				else {
					name += (next + L" ");
				}
				continue;
			}
			else if (temp == L"Heavy") {
				splitter >> next;
				if (next == L"Helmet" || next == L"Chestplate" || next == L"Leggings" || next == L"Boots") {
					name += (temp + L" " + next + L" ");
					continue;
				}
				else {
					name += (next + L" ");
				}
				continue;
			}
			else if (temp == L"Perfect") {
				splitter >> next;
				if (next == L"Helmet" || next == L"Chestplate" || next == L"Leggings" || next == L"Boots") {
					name += (temp + L" " + next + L" ");
					continue;
				}
				else {
					name += (next + L" ");
				}
				continue;
			}
			else if (temp == L"Refined") {
				splitter >> next;
				if (next == L"Mithril" || next == L"Titanium") {
					name += (temp + L" " + next + L" ");
					continue;
				}
				else {
					name += (next + L" ");
				}
				continue;
			}
			else if (temp == L"Great") {
				splitter >> next;
				if (next == L"Spook") {
					name += (temp + L" " + next + L" ");
					continue;
				}
				else {
					name += (next + L" ");
				}
				continue;
			}
			// case for Super Heavy Armor is (probably) not needed

			// strip the other reforges
			if (Item::isReforgePrefix(temp)) {
				continue;
			}
			name += (temp + L" ");
		}
		while (!name.empty() && name.back() == ' ') {
			name.pop_back();
		}
	}

	// add the rarity
	name += (L" " + rarity);
}

void Item::CalculateProfit() {
	AuctionHouse& instance = AuctionHouse::GetInstance();

	long long cachedPrice = instance.LookupPrice(this->itemName);
	this->profit = cachedPrice - this->price;

	// tax
	this->profit -= cachedPrice * 0.025f; // creation tax
	this->profit -= cachedPrice * 0.01f; // claiming tax
}

bool Item::isAlphaNumeric(const wchar_t letter) {
	if (std::ranges::find(s_alphaNumeric, letter) == s_alphaNumeric.end()) {
		return false;
	}
	return true;
}

bool Item::isDuplicatePrefix(const std::wstring& prefix) {
	if (std::ranges::find(s_duplicatePrefixes, prefix) == s_duplicatePrefixes.end()) {
		return false;
	}
	return true;
}

bool Item::isReforgePrefix(const std::wstring& prefix) {
	if (std::ranges::find(s_reforgePrefixes, prefix) == s_reforgePrefixes.end()) {
		return false;
	}
	return true;
}

bool Item::isRecombobulated(const std::wstring& itemLore) {
	std::wstringstream splitter(itemLore);

	std::wstring temp = L"";
	while (std::getline(splitter, temp, L'§')) {
		if (temp == L"ka") {
			return true;
		}
	}
	return false;
}