#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"


namespace Formula {
	[[nodiscard]] consteval auto Constant(float value) {
		return Intermediary{
			.print = [value](const Stats::Sheet &, Step) -> std::string {
				return fmt::format("{:.2f}%", value * 100.f);
			},
			.eval = [value](const Stats::Sheet &) -> float {
				return value;
			},
		};
	}

	[[nodiscard]] consteval auto ConstantFlat(float value) {
		return Intermediary{
			.print = [value](const Stats::Sheet &, Step) -> std::string {
				return fmt::format("{:.2f}", value);
			},
			.eval = [value](const Stats::Sheet &) -> float {
				return value;
			},
		};
	}
}// namespace Formula