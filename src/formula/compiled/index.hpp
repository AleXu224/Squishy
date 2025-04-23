#pragma once

#include "formula/formulaContext.hpp"
#include "intermediary.hpp"

namespace Formula::Compiled {
	template<IntFormula T, std::ranges::random_access_range V>
	struct Index : FormulaBase<std::remove_cvref_t<decltype(std::declval<V>().at(std::declval<size_t>()))>> {
		T index;
		V indexable;

		[[nodiscard]] auto eval(const Formula::Context &context) const {
			return indexable.at(index.eval(context));
		}
	};

	// auto IndexMaker(const IntFormula auto &index, const std::ranges::random_access_range auto &indexable) {
	// 	if constexpr (std::is_same_v<std::remove_cvref_t<decltype(index)>, ConstantInt>)
	// 		return Constant<std::remove_cvref_t<decltype(indexable.at(std::declval<size_t>()))>>(indexable.at(index.value));
	// 	else
	// 		return Index{
	// 			.index = index,
	// 			.indexable = indexable
	// 		};
	// }

	template<class T>
	struct Evaluator : FormulaBase<std::remove_cvref_t<decltype(std::declval<T>().eval(std::declval<const Formula::Context &>()).eval(std::declval<const Formula::Context &>()))>> {
		T evaluated;

		[[nodiscard]] auto eval(const Context &context) const {
			return evaluated.eval(context).eval(context);
		}
	};
}// namespace Formula::Compiled