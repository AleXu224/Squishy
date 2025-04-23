#pragma once

#include "formula/formulaContext.hpp"
#include "intermediary.hpp"

namespace Formula::Compiled {
	template<ArithmeticFormula T, ArithmeticFormula V>
	struct Max : FormulaBase<typename T::Type> {
		T val1;
		V val2;

		[[nodiscard]] auto eval(const Formula::Context &context) const {
			return std::max(val1.eval(context), val2.eval(context));
		}
	};

	auto MaxMaker(const ArithmeticFormula auto &param1, const ArithmeticFormula auto &param2) {
		auto type1 = param1.getType();
		auto type2 = param2.getType();

		if (type1 == Type::constant && type2 == Type::constant) {
			auto val1 = param1.getConstantValue();
			auto val2 = param2.getConstantValue();
			if (val1 > val2)
				return param1;
			else
				return param2;
		}

		return Max{
			.val1 = param1,
			.val2 = param2,
		}
			.wrap();
	}
}// namespace Formula::Compiled