#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"


namespace Formula {
	template<class T, class U>
	struct Product {
		T val1;
		U val2;

		[[nodiscard]] inline std::string print(const Stats::Sheet &stats, Step) const {
			return fmt::format("{} * {}", val1.print(stats, Step::multiplication), val2.print(stats, Step::multiplication));
		}

		[[nodiscard]] inline float eval(const Stats::Sheet &stats) const {
			return val1.eval(stats) * val2.eval(stats);
		}
	};
}// namespace Formula