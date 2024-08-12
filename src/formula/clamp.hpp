#pragma once

#include "algorithm"
#include "fmt/core.h"
#include "formula/intermediary.hpp"


namespace Formula {
	template<class T>
	struct Clamp {
		T val1;
		float min;
		float max;
		bool isPercentage = true;

		[[nodiscard]] inline std::string print(const Stats::Sheet &stats, Step) const {
			auto val = val1.eval(stats);
			if (val >= min && val <= max) return val1.print(stats, Step::none);
			return fmt::format("clamp({}, {}, {})", val1.print(stats, Step::none), min, max);
		}

		[[nodiscard]] inline float eval(const Stats::Sheet &stats) const {
			return std::clamp(val1.eval(stats), min, max);
		}
	};
}// namespace Formula