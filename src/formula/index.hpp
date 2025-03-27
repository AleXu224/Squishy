#pragma once

#include "formula/formulaContext.hpp"
#include "intermediary.hpp"
#include "percentage.hpp"
#include "step.hpp"


namespace Formula {
	template<IntFormula T, std::ranges::random_access_range V>
	struct Index {
		T index;
		bool isPercentage = true;
		V indexable;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return Percentage({}, eval(context), isPercentage);
		}

		[[nodiscard]] auto eval(const Context &context) const {
			return indexable.at(index.eval(context));
		}
	};

	template<class T>
	struct Evaluator {
		T evaluated;
		bool isPercentage = false;

		[[nodiscard]] std::string print(const Context &context, Step) const {
			return Percentage({}, eval(context), isPercentage);
		}

		[[nodiscard]] auto eval(const Context &context) const {
			return evaluated.eval(context).eval(context);
		}
	};
}// namespace Formula