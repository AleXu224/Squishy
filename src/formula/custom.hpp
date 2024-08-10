#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"


namespace Formula {
	template<class T>
	concept EvalFuncLike = requires(T t) {
		{ t(std::declval<const Stats::Sheet &>()) } -> std::same_as<float>;
	};

	template<EvalFuncLike T>
	[[nodiscard]] constexpr auto Custom(T evalFunc, bool isPercentage = false) {
		return Intermediary{
			.print = [evalFunc, isPercentage](const Stats::Sheet &stats, Step) -> std::string {
				return fmt::format("{:2f}{}", evalFunc(stats) * (isPercentage ? 100.f : 1.f), isPercentage ? "%" : "");
			},
			.eval = [evalFunc](const Stats::Sheet &stats) -> float {
				return evalFunc(stats);
			},
		};
	}

	template<EvalFuncLike T>
	[[nodiscard]] constexpr auto Custom(std::string_view prefix, T evalFunc, bool isPercentage = false) {
		return Intermediary{
			.print = [prefix, evalFunc, isPercentage](const Stats::Sheet &stats, Step) -> std::string {
				return fmt::format("{} {:.2f}{}", prefix, evalFunc(stats) * (isPercentage ? 100.f : 1.f), isPercentage ? "%" : "");
			},
			.eval = [evalFunc](const Stats::Sheet &stats) -> float {
				return evalFunc(stats);
			},
		};
	}
}// namespace Formula