#pragma once

#include "cstdint"
#include "ranges"
#include <array>
#include <unordered_map>
#include <vector>

namespace Misc {
	struct Promotion {
		uint8_t promotion;
		uint8_t minLevel;
		uint8_t maxLevel;
		uint8_t maxSkill;
		uint8_t maxCore;
	};

	static constexpr auto promotions = std::array{
		Promotion{.promotion = 0, .minLevel = 1, .maxLevel = 10, .maxSkill = 1, .maxCore = 0},
		Promotion{.promotion = 1, .minLevel = 10, .maxLevel = 20, .maxSkill = 3, .maxCore = 1},
		Promotion{.promotion = 2, .minLevel = 20, .maxLevel = 30, .maxSkill = 5, .maxCore = 2},
		Promotion{.promotion = 3, .minLevel = 30, .maxLevel = 40, .maxSkill = 7, .maxCore = 3},
		Promotion{.promotion = 4, .minLevel = 40, .maxLevel = 50, .maxSkill = 9, .maxCore = 4},
		Promotion{.promotion = 5, .minLevel = 50, .maxLevel = 60, .maxSkill = 12, .maxCore = 6},
	};

	static inline auto maxPromotionByRarity = std::unordered_map<uint8_t, uint8_t>{
		{1, 4},
		{2, 4},
		{3, 5},
		{4, 5},
		{5, 5},
	};

	static inline std::vector<Promotion> promotionsAtLvl(uint8_t lvl, uint8_t rarity) {
		return promotions
			 | std::views::take(maxPromotionByRarity.at(rarity) + 1)
			 | std::views::filter([&](const Promotion &promotion) {
				   return promotion.minLevel <= lvl && lvl <= promotion.maxLevel;
			   })
			 | std::ranges::to<std::vector<Promotion>>();
	}
}// namespace Misc