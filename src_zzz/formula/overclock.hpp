#pragma once

#include "formula/base.hpp"
#include "stats/loadout.hpp"


namespace Formula {
	[[nodiscard]] inline const auto &_getOverclockMultiplier(const std::array<float, 5> &values, const Stats::State &source) {
		return values.at(source.loadout().engine->sheet.overclock - 1);
	}

	struct EngineMultiplierValue : FormulaBase<float, Type::constant> {
		bool isPercentage = false;
		std::array<float, 5> values;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			const auto &multiplier = _getOverclockMultiplier(values, context.source);
			return Percentage({}, multiplier, isPercentage);
		}

		[[nodiscard]] float eval(const Context &context) const {
			return _getOverclockMultiplier(values, context.source);
		}
	};

	[[nodiscard]] inline auto EngineMultiplier(auto stat, const std::array<float, 5> &values) {
		return stat * EngineMultiplierValue({}, true, values);
	}

	[[nodiscard]] inline auto EngineMultiplier(bool isPercentage, const std::array<float, 5> &values) {
		return EngineMultiplierValue({}, isPercentage, values);
	}
}// namespace Formula