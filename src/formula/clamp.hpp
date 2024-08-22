#pragma once

#include "algorithm"
#include "fmt/core.h"
#include "formula/intermediary.hpp"


namespace Formula {
	template<class T>
	struct Clamp {
		T val1;
		float min = 0.f;
		float max = 1.f;
		bool isPercentage = true;

		[[nodiscard]] inline std::string print(const Stats::Loadout &stats, const Stats::Team &team, Step) const {
			auto val = val1.eval(stats, team);
			if (val >= min && val <= max) return val1.print(stats, team, Step::none);
			return fmt::format("clamp({}, {}, {})", val1.print(stats, team, Step::none), min, max);
		}

		[[nodiscard]] inline float eval(const Stats::Loadout &stats, const Stats::Team &team) const {
			return std::clamp(val1.eval(stats, team), min, max);
		}
	};
}// namespace Formula