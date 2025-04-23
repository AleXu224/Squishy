#pragma once

#include "compiled/intermediary.hpp"
#include "fmt/core.h"
#include "formula/formulaContext.hpp"
#include "step.hpp"


namespace Formula {
	template<class T>
	concept EvalFuncLike = requires(T t) {
		{ t(std::declval<const Context &>()) } -> std::same_as<float>;
	};

	template<class T>
	concept CompileFuncLike = requires(T t) {
		{ t(std::declval<const Context &>()) } -> Compiled::FloatFormula;
	};

	template<CompileFuncLike T, EvalFuncLike V>
	struct Custom {
		T compileFunc;
		V func;
		bool isPercentage = false;

		[[nodiscard]] auto compile(const Context &context) const {
			return compileFunc(context);
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("{:1f}{}", func(context) * (isPercentage ? 100.f : 1.f), isPercentage ? "%" : "");
		}

		[[nodiscard]] float eval(const Context &context) const {
			return func(context);
		}
	};
}// namespace Formula