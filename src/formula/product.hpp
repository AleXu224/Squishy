#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"


namespace Formula {
	template<IntermediaryLike T, IntermediaryLike U>
	[[nodiscard]] consteval auto Product(T param1, U param2) {
		return Intermediary{
			.print = [param1, param2](const Stats::Sheet &stats, Step) -> std::string {
				return fmt::format("{} * {}", param1.print(stats, Step::multiplication), param2.print(stats, Step::multiplication));
			},
			.eval = [param1, param2](const Stats::Sheet &stats) -> float {
				return param1.eval(stats) * param2.eval(stats);
			},
		};
	}
}// namespace Formula