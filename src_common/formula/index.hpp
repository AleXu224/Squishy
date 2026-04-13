#pragma once

#include "base.hpp"
#include "percentage.hpp"

namespace Formula {
	template<IntFormula T, std::ranges::random_access_range V>
	struct Index : FormulaBase<std::ranges::range_value_t<V>> {
		T index;
		bool isPercentage = true;
		V indexable;

		using RetType = std::ranges::range_value_t<V>;

		NodeType<RetType> fold(const Context &ctx, const FoldArgs &args) const {
			auto foldIndex = index.fold(ctx, args);
			auto typeIndex = foldIndex.getType();

			if (typeIndex == Type::constant) {
				auto idx = foldIndex.getConstantValue();
				// Error out when given an out of bounds index, gracefully handling it would just result in hidden bugs
				return ConstantBase<RetType>{.value = indexable.at(idx)};
			}

			return Index<decltype(foldIndex), V>{
				.index = foldIndex,
				.isPercentage = isPercentage,
				.indexable = indexable,
			};
		}

		[[nodiscard]] std::string print(const Context &context, Step prevStep) const {
			auto ret = eval(context);
			if constexpr (fmt::is_formattable<decltype(ret)>::value) {
				return Percentage({}, eval(context), isPercentage);
			} else {
				return ret.print(context, prevStep);
			}
		}

		[[nodiscard]] auto eval(const Context &context) const {
			return indexable.at(index.eval(context));
		}
	};

	template<class T>
	struct Evaluator : FormulaBase<FormulaType<FormulaType<T>>> {
		T evaluated;
		bool isPercentage = false;

		using RetType = decltype(std::declval<T>().eval(std::declval<const Context &>()).eval(std::declval<const Context &>()));

		NodeType<RetType> fold(const Context &ctx, const FoldArgs &args) const {
			auto foldEval = evaluated.fold(ctx, args);
			auto typeEval = foldEval.getType();

			if (typeEval == Type::constant) {
				return foldEval.getConstantValue().fold(ctx, args);
			}

			return Evaluator<decltype(foldEval)>{
				.evaluated = foldEval,
				.isPercentage = isPercentage,
			};
		}

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return Percentage({}, eval(context), isPercentage);
		}

		[[nodiscard]] auto eval(const Context &context) const {
			return evaluated.eval(context).eval(context);
		}
	};
}// namespace Formula