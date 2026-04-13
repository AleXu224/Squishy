#pragma once

#include "formula/base.hpp"


namespace Formula {
	template<class T>
	concept EvalFuncLike = requires(T t) {
		{ t(std::declval<const Context &>()) } -> std::same_as<float>;
	};

	template<class T>
	concept FoldFuncLike = requires(T t) {
		{ t(std::declval<const Context &>(), std::declval<const FoldArgs &>()) } -> FloatFormula;
	};

	template<FoldFuncLike T, EvalFuncLike V>
	struct Custom {
		T foldFunc;
		V func;
		bool isPercentage = false;

		[[nodiscard]] auto fold(const Context &context, const FoldArgs &args) const {
			return foldFunc(context, args);
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return fmt::format("{:1f}{}", func(context) * (isPercentage ? 100.f : 1.f), isPercentage ? "%" : "");
		}

		[[nodiscard]] float eval(const Context &context) const {
			return func(context);
		}
	};
}// namespace Formula