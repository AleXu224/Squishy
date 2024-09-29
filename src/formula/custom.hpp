#pragma once

#include "fmt/core.h"
#include "formula/formulaContext.hpp"
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

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("{:1f}{}", func(context) * (isPercentage ? 100.f : 1.f), isPercentage ? "%" : "");
		}

		[[nodiscard]] float eval(const Context &context) const {
			return func(context);
		}
	};
}// namespace Formula