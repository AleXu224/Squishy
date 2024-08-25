#pragma once

#include "intermediary.hpp"
#include "stats/loadout.hpp"
#include "weapon/data.hpp"

namespace Formula {
	[[nodiscard]] inline const auto &_getRefinementMultiplier(const std::array<float, 5> &values, const Stats::Loadout &source) {
		return values.at(source.weapon.sheet.refinement);
	}

	struct WeaponMultiplierValue {
		std::array<float, 5> values;

		[[nodiscard]] inline std::string print(const Stats::Loadout &source, const Stats::Loadout &, const Stats::Team &, Step) const {
			const auto &multiplier = _getRefinementMultiplier(values, source);
			return fmt::format("{:.2f}%", multiplier * 100.f);
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &source, const Stats::Loadout &, const Stats::Team &) const {
			return _getRefinementMultiplier(values, source);
		}
	};

	[[nodiscard]] consteval auto WeaponMultiplier(::Stat stat, const std::array<float, 5> &values) {
		return Formula::Stat(stat) * WeaponMultiplierValue(values);
	}
}// namespace Formula