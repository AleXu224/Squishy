#pragma once

#include "formula/formulaContext.hpp"
#include "percentage.hpp"
#include "stats/loadout.hpp"


namespace Formula {
	[[nodiscard]] inline const auto &_getRefinementMultiplier(const std::array<float, 5> &values, const Stats::Loadout &source) {
		return values.at(source.weapon->sheet.refinement - 1);
	}

	struct WeaponMultiplierValue {
		bool isPercentage = false;
		std::array<float, 5> values;

		[[nodiscard]] Compiled::FloatNode compile(const Context &context) const {
			return Compiled::ConstantFloat{.value = _getRefinementMultiplier(values, context.source)};
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			const auto &multiplier = _getRefinementMultiplier(values, context.source);
			return Percentage({}, multiplier, isPercentage);
		}

		[[nodiscard]] float eval(const Context &context) const {
			return _getRefinementMultiplier(values, context.source);
		}
	};

	[[nodiscard]] inline auto WeaponMultiplier(auto stat, const std::array<float, 5> &values) {
		return stat * WeaponMultiplierValue(true, values);
	}

	[[nodiscard]] inline auto WeaponMultiplier(bool isPercentage, const std::array<float, 5> &values) {
		return WeaponMultiplierValue(isPercentage, values);
	}
}// namespace Formula