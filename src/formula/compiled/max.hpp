#pragma once

#include "formula/formulaContext.hpp"
#include "intermediary.hpp"

namespace Formula::Compiled {
	template<ArithmeticFormula T, ArithmeticFormula V>
	struct Max {
		T val1;
		V val2;

		[[nodiscard]] auto eval(const Formula::Context &context) const {
			return std::max(val1.eval(context), val2.eval(context));
		}

		[[nodiscard]] bool isConstant() const {
			return val1.isConstant() && val2.isConstant();
		}
	};

	auto MaxMaker(const ArithmeticFormula auto &val1, const ArithmeticFormula auto &val2) {
		if constexpr (ConstantFormula<std::remove_cvref_t<decltype(val1)>> && ConstantFormula<std::remove_cvref_t<decltype(val2)>>) {
			if (val1.value > val2.value)
				return val1;
			else
				return val2;
		} else {
			return Max{val1, val2};
		}
	}
}// namespace Formula::Compiled