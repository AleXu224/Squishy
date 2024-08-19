#pragma once

#include "intermediary.hpp"
#include "stats/loadout.hpp"
#include "weapon/data.hpp"

namespace Formula {
	[[nodiscard]] inline const auto &_getRefinementMultiplier(const std::array<float, 5> &values, const Stats::Loadout &stats) {
		return values.at(stats.weapon.sheet.refinement);
	}

	struct WeaponMultiplierValue {
		std::array<float, 5> values;

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, Step) const {
			auto &multiplier = _getRefinementMultiplier(values, stats);
			return fmt::format("{:.2f}%", multiplier * 100.f);
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &stats) const {
			return _getRefinementMultiplier(values, stats);
		}
	};

	[[nodiscard]] consteval auto WeaponMultiplier(::Stat stat, const std::array<float, 5> &values) {
		return Formula::Stat(stat) * WeaponMultiplierValue(values);
	}
}// namespace Formula