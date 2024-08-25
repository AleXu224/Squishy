#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"


namespace Formula {
	template<FloatFormula T, FloatFormula U>
	struct Product {
		T val1;
		U val2;

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, const Stats::Team &team, Step prevStep) const {
			auto r1 = val1.eval(stats, team);
			auto r2 = val2.eval(stats, team);

			if (r1 == 1.f) return fmt::format("{}", val2.print(stats, team, prevStep));
			if (r2 == 1.f) return fmt::format("{}", val1.print(stats, team, prevStep));

			return fmt::format("{} * {}", val1.print(stats, team, Step::multiplication), val2.print(stats, team, Step::multiplication));
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &stats, const Stats::Team &team) const {
			return val1.eval(stats, team) * val2.eval(stats, team);
		}
	};
}// namespace Formula