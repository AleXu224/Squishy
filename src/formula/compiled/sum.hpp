#pragma once

#include "formula/formulaContext.hpp"
#include "intermediary.hpp"

namespace Formula::Compiled {
	template<ArithmeticFormula T, ArithmeticFormula V>
	struct Sum : FormulaBase<SumType<typename T::Type, typename V::Type>> {
		T val1;
		V val2;

		[[nodiscard]] auto eval(const Formula::Context &context) const {
			return val1.eval(context) + val2.eval(context);
		}
	};
}// namespace Formula::Compiled