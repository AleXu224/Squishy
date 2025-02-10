#pragma once

#include "cstdint"
#include "ranges"
#include <array>
#include <unordered_map>
#include <vector>

namespace Misc {
	struct Ascension {
		uint8_t ascension;
		uint8_t minLevel;
		uint8_t maxLevel;
		uint8_t maxTalent;
	};

	static constexpr auto ascensions = std::array{
		Ascension{.ascension = 0, .minLevel = 0, .maxLevel = 20, .maxTalent = 1},
		Ascension{.ascension = 1, .minLevel = 20, .maxLevel = 40, .maxTalent = 1},
		Ascension{.ascension = 2, .minLevel = 40, .maxLevel = 50, .maxTalent = 2},
		Ascension{.ascension = 3, .minLevel = 50, .maxLevel = 60, .maxTalent = 4},
		Ascension{.ascension = 4, .minLevel = 60, .maxLevel = 70, .maxTalent = 6},
		Ascension{.ascension = 5, .minLevel = 70, .maxLevel = 80, .maxTalent = 8},
		Ascension{.ascension = 6, .minLevel = 80, .maxLevel = 90, .maxTalent = 10},
	};

	static inline auto maxAscensionByRarity = std::unordered_map<uint8_t, uint8_t>{
		{1, 4},
		{2, 4},
		{3, 4},
		{4, 6},
		{5, 6},
	};

	static inline auto ascensionsAtLvl(uint8_t lvl, uint8_t rarity) {
		return ascensions
			 | std::views::take(maxAscensionByRarity.at(rarity) + 1)
			 | std::views::filter([&](const Ascension &ascension) {
				   return ascension.minLevel <= lvl && lvl <= ascension.maxLevel;
			   })
			 | std::ranges::to<std::vector<Ascension>>();
	}
}// namespace Misc