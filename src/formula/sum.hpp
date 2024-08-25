#pragma once

#include "fmt/format.h"
#include "intermediary.hpp"


namespace Formula {
	template<FloatFormula T, FloatFormula U>
	struct Sum {
		T val1;
		U val2;

		[[nodiscard]] inline std::string print(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team, Step prevStep) const {
			auto p1 = val1.print(source, target, team, Step::addition);
			auto p2 = val2.print(source, target, team, Step::addition);

			if (val1.eval(source, target, team) == 0.f) return p2;
			if (val2.eval(source, target, team) == 0.f) return p1;

			if (prevStep == Step::multiplication) {
				return fmt::format("({} + {})", p1, p2);
			}
			return fmt::format("{} + {}", p1, p2);
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &source, const Stats::Loadout &target, const Stats::Team &team) const {
			return val1.eval(source, target, team) + val2.eval(source, target, team);
		}
	};
}// namespace Formula