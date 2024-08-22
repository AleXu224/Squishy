#pragma once

#include "fmt/format.h"
#include "intermediary.hpp"


namespace Formula {
	template<class T, class U>
	struct Sum {
		T val1;
		U val2;

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, const Stats::Team &team, Step prevStep) const {
			auto p1 = val1.print(stats, team, Step::addition);
			auto p2 = val2.print(stats, team, Step::addition);

			if (val1.eval(stats, team) == 0.f) return p2;
			if (val2.eval(stats, team) == 0.f) return p1;

			if (prevStep == Step::multiplication) {
				return fmt::format("({} + {})", p1, p2);
			}
			return fmt::format("{} + {}", p1, p2);
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &stats, const Stats::Team &team) const {
			return val1.eval(stats, team) + val2.eval(stats, team);
		}
	};
}// namespace Formula