#pragma once

#include "fmt/format.h"
#include "intermediary.hpp"


namespace Formula {
	template<class T, class U>
	struct Sum {
		T val1;
		U val2;

		[[nodiscard]] inline std::string print(const Stats::Sheet &stats, Step prevStep) const {
			auto p1 = val1.print(stats, Step::addition);
			auto p2 = val2.print(stats, Step::addition);

			if (val1.eval(stats) == 0.f) return p2;
			if (val2.eval(stats) == 0.f) return p1;

			if (prevStep == Step::multiplication) {
				return fmt::format("({} + {})", p1, p2);
			} else {
				return fmt::format("{} + {}", p1, p2);
			}
		}

		[[nodiscard]] inline float eval(const Stats::Sheet &stats) const {
			return val1.eval(stats) + val2.eval(stats);
		}
	};
}// namespace Formula