#pragma once

#include "fmt/format.h"
#include "intermediary.hpp"


namespace Formula {
	template<IntermediaryLike T, IntermediaryLike U>
	[[nodiscard]] consteval auto Sum(T param1, U param2) {
		return Intermediary{
			.print = [param1, param2](const Stats::Sheet &stats, Step prevStep) -> std::string {
				if (prevStep == Step::multiplication) {
					return fmt::format("({} + {})", param1.print(stats, Step::addition), param2.print(stats, Step::addition));
				} else {
					return fmt::format("{} + {}", param1.print(stats, Step::addition), param2.print(stats, Step::addition));
				}
			},
			.eval = [param1, param2](const Stats::Sheet &stats) -> float {
				return param1.eval(stats) + param2.eval(stats);
			},
		};
	}
}// namespace Formula