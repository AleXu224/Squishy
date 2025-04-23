#pragma once

#include "comparators.hpp"
#include "constant.hpp"
#include "difference.hpp"
#include "fraction.hpp"
#include "intermediary.hpp"
#include "monomial.hpp"
#include "product.hpp"
#include "sum.hpp"


namespace Formula::Compiled::Operators {
	// Add
	template<ArithmeticFormula T, ArithmeticFormula V>
	[[nodiscard]] constexpr NodeType<SumType<typename T::Type, typename V::Type>> operator+(const T &param1, const V &param2) {
		auto type1 = param1.getType();
		auto type2 = param2.getType();

		if (type1 == Type::constant && type2 == Type::constant) {
			return Constant{.value = param1.getConstantValue() + param2.getConstantValue()};
		} else if (type1 == Type::constant && type2 == Type::none) {
			return SumMonomial{.value = param2}.add(param1.getConstantValue()).wrap();
		} else if (type2 == Type::constant && type1 == Type::none) {
			return SumMonomial{.value = param1}.add(param2.getConstantValue()).wrap();
		} else if (type1 == Type::constant && (type2 == Type::monomial || type2 == Type::summonomial || type2 == Type::prodmonomial)) {
			return param2.add(param1.getConstantValue());
		} else if (type2 == Type::constant && (type1 == Type::monomial || type1 == Type::summonomial || type1 == Type::prodmonomial)) {
			return param1.add(param2.getConstantValue());
		} else if (type1 == Type::summonomial && type2 == Type::summonomial) {
			return SumMonomial{.value = param1.getMonomialValue() + param2.getMonomialValue()}.add(param1.getSumParam() + param2.getSumParam()).wrap();
		}

		return Sum{
			.val1 = param1,
			.val2 = param2,
		};
	}

	// Subtract
	template<ArithmeticFormula T, ArithmeticFormula V>
	[[nodiscard]] constexpr NodeType<DifferenceType<typename T::Type, typename V::Type>> operator-(const T &param1, const V &param2) {
		auto type1 = param1.getType();
		auto type2 = param2.getType();

		if (type1 == Type::constant && type2 == Type::constant) {
			return Constant{.value = param1.getConstantValue() - param2.getConstantValue()};
		} else if (type1 == Type::constant && type2 == Type::none) {
			//a - b = b * -1 + a
			return Monomial{.value = param2}.mult(-1).add(param1.getConstantValue()).wrap();
		} else if (type2 == Type::constant && type1 == Type::none) {
			return SumMonomial{.value = param1}.subtract(param2.getConstantValue()).wrap();
		} else if (type1 == Type::constant && (type2 == Type::monomial || type2 == Type::summonomial || type2 == Type::prodmonomial)) {
			//a - b = b * -1 + a
			return param2.mult(-1).add(param1.getConstantValue());
		} else if (type2 == Type::constant && (type1 == Type::monomial || type1 == Type::summonomial || type1 == Type::prodmonomial)) {
			return param1.subtract(param2.getConstantValue());
		} else if (type1 == Type::summonomial && type2 == Type::summonomial) {
			return SumMonomial{.value = param1.getMonomialValue() - param2.getMonomialValue()}.add(param1.getSumParam() - param2.getSumParam()).wrap();
		}

		return Difference{
			.val1 = param1,
			.val2 = param2,
		};
	}

	// Multiply
	template<ArithmeticFormula T, ArithmeticFormula V>
	[[nodiscard]] constexpr NodeType<ProductType<typename T::Type, typename V::Type>> operator*(const T &param1, const V &param2) {
		auto type1 = param1.getType();
		auto type2 = param2.getType();

		if (type1 == Type::constant && type2 == Type::constant) {
			return Constant{.value = param1.getConstantValue() * param2.getConstantValue()};
		} else if (type1 == Type::constant && type2 == Type::none) {
			return ProdMonomial{.value = param2}.mult(param1.getConstantValue()).wrap();
		} else if (type2 == Type::constant && type1 == Type::none) {
			return ProdMonomial{.value = param1}.mult(param2.getConstantValue()).wrap();
		} else if (type1 == Type::constant && (type2 == Type::monomial || type2 == Type::summonomial || type2 == Type::prodmonomial)) {
			return param2.mult(param1.getConstantValue());
		} else if (type2 == Type::constant && (type1 == Type::monomial || type1 == Type::summonomial || type1 == Type::prodmonomial)) {
			return param1.mult(param2.getConstantValue());
		} else if (type1 == Type::prodmonomial && type2 == Type::prodmonomial) {
			return ProdMonomial{.value = param1.getMonomialValue() * param2.getMonomialValue()}.mult(param1.getMultParam() * param2.getMultParam()).wrap();
		}

		return Product{
			.val1 = param1,
			.val2 = param2,
		};
	}

	// Divide
	template<ArithmeticFormula T, ArithmeticFormula V>
	[[nodiscard]] constexpr NodeType<FractionType<typename T::Type, typename V::Type>> operator/(const T &param1, const V &param2) {
		auto type1 = param1.getType();
		auto type2 = param2.getType();

		if (type1 == Type::constant && type2 == Type::constant) {
			return Constant{.value = param1.getConstantValue() / param2.getConstantValue()};
		} else if (type1 == Type::constant && type2 == Type::none) {
			// return ProdMonomial{.value = param2}.divide(param1.getConstantValue()).wrap();
		} else if (type2 == Type::constant && type1 == Type::none) {
			return ProdMonomial{.value = param1}.divide(param2.getConstantValue()).wrap();
		} else if (type1 == Type::constant && (type2 == Type::monomial || type2 == Type::summonomial || type2 == Type::prodmonomial)) {
			// return param2.divide(param1.getConstantValue());
		} else if (type2 == Type::constant && (type1 == Type::monomial || type1 == Type::summonomial || type1 == Type::prodmonomial)) {
			return param1.divide(param2.getConstantValue());
		} else if (type1 == Type::prodmonomial && type2 == Type::prodmonomial) {
			return ProdMonomial{.value = param1.getMonomialValue() / param2.getMonomialValue()}.mult(param1.getMultParam() / param2.getMultParam()).wrap();
		}

		return Fraction{
			.val1 = param1,
			.val2 = param2,
		};
	}

	// Equal
	[[nodiscard]] constexpr BoolNode operator==(const FormulaLike auto &param1, const FormulaLike auto &param2) {
		auto type1 = param1.getType();
		auto type2 = param2.getType();

		if (type1 == Type::constant && type2 == Type::constant) {
			return Constant{.value = param1.getConstantValue() == param2.getConstantValue()};
		}

		return Equal{.val1 = param1, .val2 = param2};
	}

	// NotEqual
	[[nodiscard]] constexpr BoolNode operator!=(const FormulaLike auto &param1, const FormulaLike auto &param2) {
		auto type1 = param1.getType();
		auto type2 = param2.getType();

		if (type1 == Type::constant && type2 == Type::constant) {
			return Constant{.value = param1.getConstantValue() != param2.getConstantValue()};
		}

		return NotEqual{.val1 = param1, .val2 = param2};
	}

	// Not
	[[nodiscard]] constexpr BoolNode operator!(const BoolFormula auto &param1) {
		auto type1 = param1.getType();

		if (type1 == Type::constant) {
			return Constant{.value = !param1.getConstantValue()};
		}

		return Not{.val = param1};
	}

	// Less
	[[nodiscard]] constexpr BoolNode operator<(const ArithmeticFormula auto &param1, const ArithmeticFormula auto &param2) {
		auto type1 = param1.getType();
		auto type2 = param2.getType();

		if (type1 == Type::constant && type2 == Type::constant) {
			return Constant{.value = param1.getConstantValue() < param2.getConstantValue()};
		}

		return Less{.val1 = param1, .val2 = param2};
	}

	// LessEqual
	[[nodiscard]] constexpr BoolNode operator<=(const ArithmeticFormula auto &param1, const ArithmeticFormula auto &param2) {
		auto type1 = param1.getType();
		auto type2 = param2.getType();

		if (type1 == Type::constant && type2 == Type::constant) {
			return Constant{.value = param1.getConstantValue() <= param2.getConstantValue()};
		}

		return LessEqual{.val1 = param1, .val2 = param2};
	}

	// Greater
	[[nodiscard]] constexpr BoolNode operator>(const ArithmeticFormula auto &param1, const ArithmeticFormula auto &param2) {
		auto type1 = param1.getType();
		auto type2 = param2.getType();

		if (type1 == Type::constant && type2 == Type::constant) {
			return Constant{.value = param1.getConstantValue() > param2.getConstantValue()};
		}

		return Greater{.val1 = param1, .val2 = param2};
	}

	// GreaterEqual
	[[nodiscard]] constexpr BoolNode operator>=(const ArithmeticFormula auto &param1, const ArithmeticFormula auto &param2) {
		auto type1 = param1.getType();
		auto type2 = param2.getType();

		if (type1 == Type::constant && type2 == Type::constant) {
			return Constant{.value = param1.getConstantValue() >= param2.getConstantValue()};
		}

		return GreaterEqual{.val1 = param1, .val2 = param2};
	}

	// And
	[[nodiscard]] constexpr BoolNode operator&&(const BoolFormula auto &param1, const BoolFormula auto &param2) {
		auto type1 = param1.getType();
		auto type2 = param2.getType();

		if (type1 == Type::constant && type2 == Type::constant) {
			return Constant{.value = param1.getConstantValue() && param2.getConstantValue()};
		} else if (type1 == Type::constant && !param1.getConstantValue()) {
			return ConstantBool{.value = false}.wrap();
		} else if (type2 == Type::constant && !param2.getConstantValue()) {
			return ConstantBool{.value = false}.wrap();
		}

		return And{.val1 = param1, .val2 = param2};
	}

	// Or
	[[nodiscard]] constexpr BoolNode operator||(const BoolFormula auto &param1, const BoolFormula auto &param2) {
		auto type1 = param1.getType();
		auto type2 = param2.getType();

		if (type1 == Type::constant && type2 == Type::constant) {
			return Constant{.value = param1.getConstantValue() || param2.getConstantValue()};
		} else if (type1 == Type::constant && param1.getConstantValue()) {
			return ConstantBool{.value = true}.wrap();
		} else if (type2 == Type::constant && param2.getConstantValue()) {
			return ConstantBool{.value = true}.wrap();
		}

		return Or{.val1 = param1, .val2 = param2};
	}
}// namespace Formula::Compiled::Operators