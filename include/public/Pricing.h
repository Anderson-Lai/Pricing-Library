#pragma once

#include <vector>
#include "Item.hpp"

namespace Pricing
{
	void Start();

	// returns a sorted vector from least to most profitable
	std::vector<Item> GetFlips();
}