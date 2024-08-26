#pragma once

#include "context.hpp"
#include "fmt/core.h"
#include "step.hpp"


namespace Formula {
	template<class T>
	concept EvalFuncLike = requires(T t) {
		{ t(std::declval<const Context &>()) } -> std::same_as<float>;
	};

	template<EvalFuncLike T>
	struct Custom {
		T func;
		bool isPercentage = false;

		[[nodiscard]] inline std::string print(const Context &context, Step) const {
			return fmt::format("{:2f}{}", func(context) * (isPercentage ? 100.f : 1.f), isPercentage ? "%" : "");
		}

		[[nodiscard]] inline float eval(const Context &context) const {
			return func(context);
		}
	};
}// namespace Formula