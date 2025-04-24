#pragma once

#include "constant.hpp"
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

		[[nodiscard]] std::string print() const {
			return fmt::format("Index<{}, ...>", index.print());
		}
	};

	auto IndexMaker(const IntFormula auto &index, const std::ranges::random_access_range auto &indexable) {
		auto type = index.getType();

		if (type == Type::constant) {
			return Constant<std::remove_cvref_t<decltype(indexable.at(std::declval<size_t>()))>>{.value = indexable.at(index.getConstantValue())}.wrap();
		}

		return Index{.index = index, .indexable = indexable}.wrap();
	}

	template<class T>
	struct Evaluator : FormulaBase<std::remove_cvref_t<decltype(std::declval<T>().eval(std::declval<const Formula::Context &>()).eval(std::declval<const Formula::Context &>()))>> {
		T evaluated;

		[[nodiscard]] auto eval(const Context &context) const {
			return evaluated.eval(context).eval(context);
		}

		[[nodiscard]] std::string print() const {
			return fmt::format("Evaluator<{}>", evaluated.print());
		}
	};

	auto EvaluatorMaker(const Context &context, const auto &evaluated) {
		auto type = evaluated.getType();

		if (type == Type::constant) {
			return evaluated.getConstantValue().compile(context);
		}

		return Evaluator{.evaluated = evaluated}.wrap();
	}
}// namespace Formula::Compiled