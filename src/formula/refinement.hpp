#pragma once

#include "intermediary.hpp"
#include "stats/sheet.hpp"
#include "weapon/data.hpp"

namespace Formula {
	[[nodiscard]] inline const auto &_getRefinementMultiplier(size_t index, const Stats::Sheet &stats) {
		return stats.weapon.data.multipliers.at(index).at(stats.weapon.sheet.refinement);
	}

	[[nodiscard]] consteval auto RefinementMultiplier(size_t index) {
		return Intermediary{
			.print = [index](const Stats::Sheet &stats, Step) -> std::string {
				auto &multiplier = _getRefinementMultiplier(index, stats);
				return fmt::format("{:.2f}%", multiplier * 100.f);
			},
			.eval = [index](const Stats::Sheet &stats) -> float {
				return _getRefinementMultiplier(index, stats);
			},
		};
	}
}// namespace Formula