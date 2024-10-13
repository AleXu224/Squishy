#pragma once

#include "cstdint"
#include "ranges"
#include <array>
#include <vector>

namespace Misc {
	struct Ascension {
		uint8_t ascension;
		uint8_t minLevel;
		uint8_t maxLevel;
	};

	static constexpr auto ascensions = std::array{
		Ascension{0, 0, 20},
		Ascension{1, 20, 40},
		Ascension{2, 40, 50},
		Ascension{3, 50, 60},
		Ascension{4, 60, 70},
		Ascension{5, 70, 80},
		Ascension{6, 80, 90},
	};

	static inline auto ascensionsAtLvl(uint8_t lvl) {
		return ascensions
			 | std::views::filter([&](const Ascension &ascension) {
				   return ascension.minLevel <= lvl && lvl <= ascension.maxLevel;
			   })
			 | std::ranges::to<std::vector<Ascension>>();
	}
}// namespace Misc