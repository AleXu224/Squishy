#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"


namespace Formula {
	template<FloatFormula T, FloatFormula U>
	struct Product {
		T val1;
		U val2;

		[[nodiscard]] inline std::string print(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team, Step prevStep) const {
			auto r1 = val1.eval(source, target, team);
			auto r2 = val2.eval(source, target, team);

			if (r1 == 1.f) return fmt::format("{}", val2.print(source, target, team, prevStep));
			if (r2 == 1.f) return fmt::format("{}", val1.print(source, target, team, prevStep));

			return fmt::format("{} * {}", val1.print(source, target, team, Step::multiplication), val2.print(source, target, team, Step::multiplication));
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team) const {
			return val1.eval(source, target, team) * val2.eval(source, target, team);
		}
	};
}// namespace Formula