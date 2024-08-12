#pragma once

#include "intermediary.hpp"
#include "stats/sheet.hpp"
#include "weapon/data.hpp"

namespace Formula {
	[[nodiscard]] inline const auto &_getRefinementMultiplier(size_t index, const Stats::Sheet &stats) {
		return stats.weapon.data.multipliers.at(index).at(stats.weapon.sheet.refinement);
	}

	struct WeaponMultiplier{
		size_t index;

		[[nodiscard]] inline std::string print(const Stats::Sheet &stats, Step) const {
			auto &multiplier = _getRefinementMultiplier(index, stats);
			return fmt::format("{:.2f}%", multiplier * 100.f);
		}

		[[nodiscard]] inline float eval(const Stats::Sheet &stats) const {
			return _getRefinementMultiplier(index, stats);
		}
	};
}// namespace Formula