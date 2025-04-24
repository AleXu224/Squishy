#pragma once

#include "formula/formulaContext.hpp"
#include "intermediary.hpp"

namespace Formula::Compiled {
	template<ArithmeticFormula T, ArithmeticFormula V>
	struct Min : FormulaBase<typename T::Type> {
		T val1;
		V val2;

		[[nodiscard]] auto eval(const Formula::Context &context) const {
			return std::min(val1.eval(context), val2.eval(context));
		}

		[[nodiscard]] std::string print() const {
			return fmt::format("Min<{}, {}>", val1.print(), val2.print());
		}
	};

	auto MinMaker(const ArithmeticFormula auto &param1, const ArithmeticFormula auto &param2) {
		auto type1 = param1.getType();
		auto type2 = param2.getType();

		if (type1 == Type::constant && type2 == Type::constant) {
			auto val1 = param1.getConstantValue();
			auto val2 = param2.getConstantValue();
			if (val1 < val2)
				return param1;
			else
				return param2;
		}

		return Min{
			.val1 = param1,
			.val2 = param2,
		}
			.wrap();
	}
	// auto MinMaker(const ArithmeticFormula auto &val1, const ArithmeticFormula auto &val2) {
	// 	if constexpr (ConstantFormula<std::remove_cvref_t<decltype(val1)>> && ConstantFormula<std::remove_cvref_t<decltype(val2)>>) {
	// 		if (val1.value < val2.value)
	// 			return val1;
	// 		else
	// 			return val2;
	// 	} else {
	// 		return Min{val1, val2};
	// 	}
	// }
}// namespace Formula::Compiled