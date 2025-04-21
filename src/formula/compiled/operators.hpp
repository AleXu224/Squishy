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
	template<MonomialFormula T>
	[[nodiscard]] constexpr auto operator+(const ArithmeticConstant auto &param1, const T &param2) {
		return param2.add(param1.value);
	}

	template<MonomialFormula T>
	[[nodiscard]] constexpr auto operator+(const T &param1, const ArithmeticConstant auto &param2) {
		return param1.add(param2.value);
	}

	[[nodiscard]] constexpr auto operator+(const ArithmeticConstant auto &param1, const ArithmeticConstant auto &param2) {
		return Constant(param1.value + param2.value);
	}

	[[nodiscard]] constexpr auto operator+(const ArithmeticConstant auto &param1, const ArithmeticVariableNotMonomial auto &param2) {
		return SumMonomial{param2}.add(param1.value);
	}

	[[nodiscard]] constexpr auto operator+(const ArithmeticVariableNotMonomial auto &param1, const ArithmeticConstant auto &param2) {
		return SumMonomial{param1}.add(param2.value);
	}

	[[nodiscard]] constexpr auto operator+(const ArithmeticVariableFormula auto &param1, const ArithmeticVariableFormula auto &param2) {
		if constexpr (SumOnlyMonomial<decltype(param1)> && SumOnlyMonomial<decltype(param2)>) {
			return SumMonomial{param1.value + param2.value}.add(param1.sumParam + param2.param1.sumParam);
		} else {
			return Sum(param1, param2);
		}
	}

	// Subtract
	template<MonomialFormula T>
	[[nodiscard]] constexpr auto operator-(const ConstantFormula auto &param1, const T &param2) {
		return param2.subtract(param1.value);
	}

	template<MonomialFormula T>
	[[nodiscard]] constexpr auto operator-(const T &param1, const ConstantFormula auto &param2) {
		return param1.subtract(param2.value);
	}

	[[nodiscard]] constexpr auto operator-(const ConstantFormula auto &param1, const ArithmeticVariableNotMonomial auto &param2) {
		return SumMonomial{param2}.subtract(param1.value);
	}

	[[nodiscard]] constexpr auto operator-(const ArithmeticVariableNotMonomial auto &param1, const ConstantFormula auto &param2) {
		return SumMonomial{param1}.subtract(param2.value);
	}

	[[nodiscard]] constexpr auto operator-(const ArithmeticConstant auto &param1, const ArithmeticConstant auto &param2) {
		return Constant(param1.value - param2.value);
	}

	[[nodiscard]] constexpr auto operator-(const ArithmeticVariableFormula auto &param1, const ArithmeticVariableFormula auto &param2) {
		if constexpr (SumOnlyMonomial<decltype(param1)> && SumOnlyMonomial<decltype(param2)>) {
			return SumMonomial{param1.value - param2.value}.add(param1.sumParam - param2.param1.sumParam);
		} else {
			return Difference(param1, param2);
		}
	}

	// Multiply
	template<MonomialFormula T>
	[[nodiscard]] constexpr auto operator*(const ConstantFormula auto &param1, const T &param2) {
		return param2.mult(param1.value);
	}

	template<MonomialFormula T>
	[[nodiscard]] constexpr auto operator*(const T &param1, const ConstantFormula auto &param2) {
		return param1.mult(param2.value);
	}

	[[nodiscard]] constexpr auto operator*(const ConstantFormula auto &param1, const ArithmeticVariableNotMonomial auto &param2) {
		return ProdMonomial{param2}.mult(param1.value);
	}

	[[nodiscard]] constexpr auto operator*(const ArithmeticVariableNotMonomial auto &param1, const ConstantFormula auto &param2) {
		return ProdMonomial{param1}.mult(param2.value);
	}

	[[nodiscard]] constexpr auto operator*(const ArithmeticConstant auto &param1, const ArithmeticConstant auto &param2) {
		return Constant(param1.value * param2.value);
	}

	[[nodiscard]] constexpr auto operator*(const ArithmeticVariableFormula auto &param1, const ArithmeticVariableFormula auto &param2) {
		if constexpr (ProdOnlyMonomial<decltype(param1)> && ProdOnlyMonomial<decltype(param2)>) {
			return ProdMonomial{param1.value * param2.value}.mult(param1.multParam * param2.param1.multParam);
		} else {
			return Product(param1, param2);
		}
	}

	// Divide
	template<MonomialFormula T>
	[[nodiscard]] constexpr auto operator/(const ConstantFormula auto &param1, const T &param2) {
		return Fraction{param1, param2};
	}

	template<MonomialFormula T>
	[[nodiscard]] constexpr auto operator/(const T &param1, const ConstantFormula auto &param2) {
		return param1.divide(param2.value);
	}

	[[nodiscard]] constexpr auto operator/(const ConstantFormula auto &param1, const ArithmeticVariableNotMonomial auto &param2) {
		return Fraction{param1, param2};
	}

	[[nodiscard]] constexpr auto operator/(const ArithmeticVariableNotMonomial auto &param1, const ConstantFormula auto &param2) {
		return ProdMonomial{param1}.divide(param2.value);
	}

	[[nodiscard]] constexpr auto operator/(const ArithmeticConstant auto &param1, const ArithmeticConstant auto &param2) {
		return Constant(param1.value / param2.value);
	}

	[[nodiscard]] constexpr auto operator/(const ArithmeticVariableFormula auto &param1, const ArithmeticVariableFormula auto &param2) {
		if constexpr (ProdOnlyMonomial<decltype(param1)> && ProdOnlyMonomial<decltype(param2)>) {
			return ProdMonomial{param1.value / param2.value}.mult(param1.multParam / param2.param1.multParam);
		} else {
			return Fraction(param1, param2);
		}
	}

	// Equal
	[[nodiscard]] constexpr auto operator==(const ConstantFormula auto &param1, const ConstantFormula auto &param2) {
		return ConstantBool(param1.value == param2.value);
	}

	[[nodiscard]] constexpr auto operator==(const FormulaLike auto &param1, const FormulaLike auto &param2)
		requires OneVariable<decltype(param1), decltype(param2)>
	{
		return Equal(param1, param2);
	}

	// NotEqual
	[[nodiscard]] constexpr auto operator!=(const ConstantFormula auto &param1, const ConstantFormula auto &param2) {
		return ConstantBool(param1.value != param2.value);
	}

	[[nodiscard]] constexpr auto operator!=(const FormulaLike auto &param1, const FormulaLike auto &param2)
		requires OneVariable<decltype(param1), decltype(param2)>
	{
		return NotEqual(param1, param2);
	}

	// Not
	[[nodiscard]] constexpr auto operator!(const ConstantBool &param1) {
		return ConstantBool(!param1.value);
	}

	[[nodiscard]] constexpr auto operator!(const BoolFormula auto &param1) {
		return Not(param1);
	}

	// Less
	[[nodiscard]] constexpr auto operator<(const ArithmeticConstant auto &param1, const ArithmeticConstant auto &param2) {
		return ConstantBool(param1.value < param2.value);
	}

	[[nodiscard]] constexpr auto operator<(const ArithmeticFormula auto &param1, const ArithmeticFormula auto &param2)
		requires OneVariable<decltype(param1), decltype(param2)>
	{
		return Less(param1, param2);
	}

	// LessEqual
	[[nodiscard]] constexpr auto operator<=(const ArithmeticConstant auto &param1, const ArithmeticConstant auto &param2) {
		return ConstantBool(param1.value <= param2.value);
	}

	[[nodiscard]] constexpr auto operator<=(const ArithmeticFormula auto &param1, const ArithmeticFormula auto &param2)
		requires OneVariable<decltype(param1), decltype(param2)>
	{
		return LessEqual(param1, param2);
	}

	// Greater
	[[nodiscard]] constexpr auto operator>(const ArithmeticConstant auto &param1, const ArithmeticConstant auto &param2) {
		return ConstantBool(param1.value > param2.value);
	}

	[[nodiscard]] constexpr auto operator>(const ArithmeticFormula auto &param1, const ArithmeticFormula auto &param2)
		requires OneVariable<decltype(param1), decltype(param2)>
	{
		return Greater(param1, param2);
	}

	// GreaterEqual
	[[nodiscard]] constexpr auto operator>=(const ArithmeticConstant auto &param1, const ArithmeticConstant auto &param2) {
		return ConstantBool(param1.value >= param2.value);
	}

	[[nodiscard]] constexpr auto operator>=(const ArithmeticFormula auto &param1, const ArithmeticFormula auto &param2)
		requires OneVariable<decltype(param1), decltype(param2)>
	{
		return GreaterEqual(param1, param2);
	}

	// And
	[[nodiscard]] constexpr auto operator&&(const ConstantBool &param1, const ConstantBool &param2) {
		return ConstantBool(param1.value && param2.value);
	}

	[[nodiscard]] constexpr auto operator&&(const BoolFormula auto &param1, const BoolFormula auto &param2) {
		return And(param1, param2);
	}

	// Or
	[[nodiscard]] constexpr auto operator||(const ConstantBool &param1, const ConstantBool &param2) {
		return ConstantBool(param1.value || param2.value);
	}

	[[nodiscard]] constexpr auto operator||(const BoolFormula auto &param1, const BoolFormula auto &param2) {
		return Or(param1, param2);
	}
}// namespace Formula::Compiled::Operators