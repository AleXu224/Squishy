#pragma once

#include "comparators.hpp"
#include "constant.hpp"
#include "fraction.hpp"
#include "product.hpp"
#include "sum.hpp"


namespace Formula {
	// Sum
	template<FloatFormula T, FloatFormula U>
	[[nodiscard]] constexpr auto operator+(const T &param1, const U &param2) {
		return Sum(param1, param2);
	}

	template<FloatFormula T>
	[[nodiscard]] constexpr auto operator+(const float &param1, const T &param2) {
		return Sum(Constant(param1), param2);
	}

	template<FloatFormula T>
	[[nodiscard]] constexpr auto operator+(const T &param1, const float &param2) {
		return Sum(param1, Constant(param2));
	}

	// Product
	template<FloatFormula T, FloatFormula U>
	[[nodiscard]] constexpr auto operator*(T param1, U param2) {
		return Product(param1, param2);
	}

	template<FloatFormula T>
	[[nodiscard]] constexpr auto operator*(float param1, T param2) {
		return Product(Constant(param1), param2);
	}

	template<FloatFormula T>
	[[nodiscard]] constexpr auto operator*(T param1, float param2) {
		return Product(param1, Constant(param2));
	}

	// Fraction
	template<FloatFormula T, FloatFormula U>
	[[nodiscard]] constexpr auto operator/(T param1, U param2) {
		return Fraction(param1, param2);
	}

	template<FloatFormula T>
	[[nodiscard]] constexpr auto operator/(float param1, T param2) {
		return Fraction(Constant(param1), param2);
	}

	template<FloatFormula T>
	[[nodiscard]] constexpr auto operator/(T param1, float param2) {
		return Fraction(param1, Constant(param2));
	}

	// Equal
	template<FormulaLike T, FormulaLike U>
	[[nodiscard]] constexpr auto operator==(T param1, U param2) {
		return Equal(param1, param2);
	}
	template<FloatFormula T>
	[[nodiscard]] constexpr auto operator==(T param1, float param2) {
		return Equal(param1, Constant(param2));
	}
	template<FloatFormula T>
	[[nodiscard]] constexpr auto operator==(float param1, T param2) {
		return Equal(Constant(param1), param2);
	}
	template<BoolFormula T>
	[[nodiscard]] constexpr auto operator==(T param1, bool param2) {
		return Equal(param1, ConstantBool(param2));
	}
	template<BoolFormula T>
	[[nodiscard]] constexpr auto operator==(bool param1, T param2) {
		return Equal(ConstantBool(param1), param2);
	}

	// NotEqual
	template<FormulaLike T, FormulaLike U>
	[[nodiscard]] constexpr auto operator!=(T param1, U param2) {
		return NotEqual(param1, param2);
	}
	template<FloatFormula T>
	[[nodiscard]] constexpr auto operator!=(T param1, float param2) {
		return NotEqual(param1, Constant(param2));
	}
	template<FloatFormula T>
	[[nodiscard]] constexpr auto operator!=(float param1, T param2) {
		return NotEqual(Constant(param1), param2);
	}
	template<BoolFormula T>
	[[nodiscard]] constexpr auto operator!=(T param1, bool param2) {
		return NotEqual(param1, ConstantBool(param2));
	}
	template<BoolFormula T>
	[[nodiscard]] constexpr auto operator!=(bool param1, T param2) {
		return NotEqual(ConstantBool(param1), param2);
	}

	// Not
	template<BoolFormula T>
	[[nodiscard]] constexpr auto operator!(T param) {
		return Not(param);
	}

	// Less
	template<FloatFormula T, FloatFormula U>
	[[nodiscard]] constexpr auto operator<(T param1, U param2) {
		return Less(param1, param2);
	}
	template<FloatFormula T>
	[[nodiscard]] constexpr auto operator<(T param1, float param2) {
		return Less(param1, Constant(param2));
	}
	template<FloatFormula T>
	[[nodiscard]] constexpr auto operator<(float param1, T param2) {
		return Less(Constant(param1), param2);
	}

	// Less Equal
	template<FloatFormula T, FloatFormula U>
	[[nodiscard]] constexpr auto operator<=(T param1, U param2) {
		return LessEqual(param1, param2);
	}
	template<FloatFormula T>
	[[nodiscard]] constexpr auto operator<=(T param1, float param2) {
		return LessEqual(param1, Constant(param2));
	}
	template<FloatFormula T>
	[[nodiscard]] constexpr auto operator<=(float param1, T param2) {
		return LessEqual(Constant(param1), param2);
	}

	// Greater
	template<FloatFormula T, FloatFormula U>
	[[nodiscard]] constexpr auto operator>(T param1, U param2) {
		return Greater(param1, param2);
	}
	template<FloatFormula T>
	[[nodiscard]] constexpr auto operator>(T param1, float param2) {
		return Greater(param1, Constant(param2));
	}
	template<FloatFormula T>
	[[nodiscard]] constexpr auto operator>(float param1, T param2) {
		return Greater(Constant(param1), param2);
	}

	// Greater Equal
	template<FloatFormula T, FloatFormula U>
	[[nodiscard]] constexpr auto operator>=(T param1, U param2) {
		return GreaterEqual(param1, param2);
	}
	template<FloatFormula T>
	[[nodiscard]] constexpr auto operator>=(T param1, float param2) {
		return GreaterEqual(param1, Constant(param2));
	}
	template<FloatFormula T>
	[[nodiscard]] constexpr auto operator>=(float param1, T param2) {
		return GreaterEqual(Constant(param1), param2);
	}

	// And
	template<BoolFormula T, BoolFormula U>
	[[nodiscard]] constexpr auto operator&&(T param1, U param2) {
		return And(param1, param2);
	}
	template<BoolFormula T>
	[[nodiscard]] constexpr auto operator&&(T param1, bool param2) {
		return And(param1, ConstantBool(param2));
	}
	template<BoolFormula T>
	[[nodiscard]] constexpr auto operator&&(bool param1, T param2) {
		return And(ConstantBool(param1), param2);
	}

	// Or
	template<BoolFormula T, BoolFormula U>
	[[nodiscard]] constexpr auto operator||(T param1, U param2) {
		return Or(param1, param2);
	}
	template<BoolFormula T>
	[[nodiscard]] constexpr auto operator||(T param1, bool param2) {
		return Or(param1, ConstantBool(param2));
	}
	template<BoolFormula T>
	[[nodiscard]] constexpr auto operator||(bool param1, T param2) {
		return Or(ConstantBool(param1), param2);
	}
}// namespace Formula