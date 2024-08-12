#pragma once

#include "fmt/core.h"
#include "intermediary.hpp"


namespace Formula {
	template<class T>
	concept EvalFuncLike = requires(T t) {
		{ t(std::declval<const Stats::Sheet &>()) } -> std::same_as<float>;
	};

	template<EvalFuncLike T>
	struct Custom {
		std::string_view prefix{};
		T func;
		bool isPercentage = false;

		[[nodiscard]] inline std::string print(const Stats::Sheet &stats, Step) const {
			return fmt::format("{}{}{:2f}{}", prefix, prefix.empty() ? "" : " ", func(stats) * (isPercentage ? 100.f : 1.f), isPercentage ? "%" : "");
		}

		[[nodiscard]] inline float eval(const Stats::Sheet &stats) const {
			return func(stats);
		}
	};
}// namespace Formula