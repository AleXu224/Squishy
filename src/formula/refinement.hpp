#pragma once

#include "formula/formulaContext.hpp"
#include "stats/loadout.hpp"


namespace Formula {
	[[nodiscard]] inline const auto &_getRefinementMultiplier(const std::array<float, 5> &values, const Stats::Loadout &source) {
		return values.at(source.weapon.sheet.refinement - 1);
	}

	struct WeaponMultiplierValue {
		std::array<float, 5> values;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			const auto &multiplier = _getRefinementMultiplier(values, context.source);
			return fmt::format("{:.1f}%", multiplier * 100.f);
		}

		[[nodiscard]] float eval(const Context &context) const {
			return _getRefinementMultiplier(values, context.source);
		}
	};

	[[nodiscard]] consteval auto WeaponMultiplier(auto stat, const std::array<float, 5> &values) {
		return stat * WeaponMultiplierValue(values);
	}
}// namespace Formula