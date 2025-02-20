#pragma once

#include "comparators.hpp"
#include "constant.hpp"
#include "fraction.hpp"
#include "product.hpp"
#include "sum.hpp"

namespace Formula::Operators {
	// Sum
	template<Formula::ArithmeticFormula T, Formula::ArithmeticFormula U>
	[[nodiscard]] constexpr auto operator+(const T &param1, const U &param2) {
		return Formula::Sum(param1, param2);
	}

	template<Formula::ArithmeticFormula T>
	[[nodiscard]] constexpr auto operator+(const float &param1, const T &param2) {
		return Formula::Sum(Formula::Constant(param1), param2);
	}

	template<Formula::ArithmeticFormula T>
	[[nodiscard]] constexpr auto operator+(const T &param1, const float &param2) {
		return Formula::Sum(param1, Formula::Constant(param2));
	}

	// Product
	template<Formula::ArithmeticFormula T, Formula::ArithmeticFormula U>
	[[nodiscard]] constexpr auto operator*(T param1, U param2) {
		return Formula::Product(param1, param2);
	}

	template<Formula::ArithmeticFormula T>
	[[nodiscard]] constexpr auto operator*(float param1, T param2) {
		return Formula::Product(Formula::Constant(param1), param2);
	}

	template<Formula::ArithmeticFormula T>
	[[nodiscard]] constexpr auto operator*(T param1, float param2) {
		return Formula::Product(param1, Formula::Constant(param2));
	}

	// Fraction
	template<Formula::ArithmeticFormula T, Formula::ArithmeticFormula U>
	[[nodiscard]] constexpr auto operator/(T param1, U param2) {
		return Formula::Fraction(param1, param2);
	}

	template<Formula::ArithmeticFormula T>
	[[nodiscard]] constexpr auto operator/(float param1, T param2) {
		return Formula::Fraction(Formula::Constant(param1), param2);
	}

	template<Formula::ArithmeticFormula T>
	[[nodiscard]] constexpr auto operator/(T param1, float param2) {
		return Formula::Fraction(param1, Formula::Constant(param2));
	}

	// Equal
	template<Formula::FormulaLike T, Formula::FormulaLike U>
	[[nodiscard]] constexpr auto operator==(T param1, U param2) {
		return Formula::Equal(param1, param2);
	}
	template<Formula::FloatFormula T>
	[[nodiscard]] constexpr auto operator==(T param1, float param2) {
		return Formula::Equal(param1, Formula::Constant(param2));
	}
	template<Formula::FloatFormula T>
	[[nodiscard]] constexpr auto operator==(float param1, T param2) {
		return Formula::Equal(Formula::Constant(param1), param2);
	}
	template<Formula::IntFormula T>
	[[nodiscard]] constexpr auto operator==(T param1, uint32_t param2) {
		return Formula::Equal(param1, Formula::ConstantInt(param2));
	}
	template<Formula::IntFormula T>
	[[nodiscard]] constexpr auto operator==(uint32_t param1, T param2) {
		return Formula::Equal(Formula::ConstantInt(param1), param2);
	}
	template<Formula::BoolFormula T>
	[[nodiscard]] constexpr auto operator==(T param1, bool param2) {
		return Formula::Equal(param1, Formula::ConstantBool(param2));
	}
	template<Formula::BoolFormula T>
	[[nodiscard]] constexpr auto operator==(bool param1, T param2) {
		return Formula::Equal(Formula::ConstantBool(param1), param2);
	}

	// NotEqual
	template<Formula::FormulaLike T, Formula::FormulaLike U>
	[[nodiscard]] constexpr auto operator!=(T param1, U param2) {
		return Formula::NotEqual(param1, param2);
	}
	template<Formula::FloatFormula T>
	[[nodiscard]] constexpr auto operator!=(T param1, float param2) {
		return Formula::NotEqual(param1, Formula::Constant(param2));
	}
	template<Formula::FloatFormula T>
	[[nodiscard]] constexpr auto operator!=(float param1, T param2) {
		return Formula::NotEqual(Formula::Constant(param1), param2);
	}
	template<Formula::IntFormula T>
	[[nodiscard]] constexpr auto operator!=(T param1, uint32_t param2) {
		return Formula::NotEqual(param1, Formula::ConstantInt(param2));
	}
	template<Formula::IntFormula T>
	[[nodiscard]] constexpr auto operator!=(uint32_t param1, T param2) {
		return Formula::NotEqual(Formula::ConstantInt(param1), param2);
	}
	template<Formula::BoolFormula T>
	[[nodiscard]] constexpr auto operator!=(T param1, bool param2) {
		return Formula::NotEqual(param1, Formula::ConstantBool(param2));
	}
	template<Formula::BoolFormula T>
	[[nodiscard]] constexpr auto operator!=(bool param1, T param2) {
		return Formula::NotEqual(Formula::ConstantBool(param1), param2);
	}

	// Not
	template<Formula::BoolFormula T>
	[[nodiscard]] constexpr auto operator!(T param) {
		return Formula::Not(param);
	}

	// Less
	template<Formula::FloatFormula T, Formula::FloatFormula U>
	[[nodiscard]] constexpr auto operator<(T param1, U param2) {
		return Formula::Less(param1, param2);
	}
	template<Formula::FloatFormula T>
	[[nodiscard]] constexpr auto operator<(T param1, float param2) {
		return Formula::Less(param1, Formula::Constant(param2));
	}
	template<Formula::FloatFormula T>
	[[nodiscard]] constexpr auto operator<(float param1, T param2) {
		return Formula::Less(Formula::Constant(param1), param2);
	}
	template<Formula::IntFormula T>
	[[nodiscard]] constexpr auto operator<(T param1, uint32_t param2) {
		return Formula::Less(param1, Formula::ConstantInt(param2));
	}
	template<Formula::IntFormula T>
	[[nodiscard]] constexpr auto operator<(uint32_t param1, T param2) {
		return Formula::Less(Formula::ConstantInt(param1), param2);
	}

	// Less Equal
	template<Formula::FloatFormula T, Formula::FloatFormula U>
	[[nodiscard]] constexpr auto operator<=(T param1, U param2) {
		return Formula::LessEqual(param1, param2);
	}
	template<Formula::FloatFormula T>
	[[nodiscard]] constexpr auto operator<=(T param1, float param2) {
		return Formula::LessEqual(param1, Formula::Constant(param2));
	}
	template<Formula::FloatFormula T>
	[[nodiscard]] constexpr auto operator<=(float param1, T param2) {
		return Formula::LessEqual(Formula::Constant(param1), param2);
	}
	template<Formula::IntFormula T>
	[[nodiscard]] constexpr auto operator<=(T param1, uint32_t param2) {
		return Formula::LessEqual(param1, Formula::ConstantInt(param2));
	}
	template<Formula::IntFormula T>
	[[nodiscard]] constexpr auto operator<=(uint32_t param1, T param2) {
		return Formula::LessEqual(Formula::ConstantInt(param1), param2);
	}

	// Greater
	template<Formula::FloatFormula T, Formula::FloatFormula U>
	[[nodiscard]] constexpr auto operator>(T param1, U param2) {
		return Formula::Greater(param1, param2);
	}
	template<Formula::FloatFormula T>
	[[nodiscard]] constexpr auto operator>(T param1, float param2) {
		return Formula::Greater(param1, Formula::Constant(param2));
	}
	template<Formula::FloatFormula T>
	[[nodiscard]] constexpr auto operator>(float param1, T param2) {
		return Formula::Greater(Formula::Constant(param1), param2);
	}
	template<Formula::IntFormula T>
	[[nodiscard]] constexpr auto operator>(T param1, uint32_t param2) {
		return Formula::Greater(param1, Formula::ConstantInt(param2));
	}
	template<Formula::IntFormula T>
	[[nodiscard]] constexpr auto operator>(uint32_t param1, T param2) {
		return Formula::Greater(Formula::ConstantInt(param1), param2);
	}

	// Greater Equal
	template<Formula::FloatFormula T, Formula::FloatFormula U>
	[[nodiscard]] constexpr auto operator>=(T param1, U param2) {
		return GreaterEqual(param1, param2);
	}
	template<Formula::FloatFormula T>
	[[nodiscard]] constexpr auto operator>=(T param1, float param2) {
		return GreaterEqual(param1, Formula::Constant(param2));
	}
	template<Formula::FloatFormula T>
	[[nodiscard]] constexpr auto operator>=(float param1, T param2) {
		return GreaterEqual(Formula::Constant(param1), param2);
	}
	template<Formula::IntFormula T>
	[[nodiscard]] constexpr auto operator>=(T param1, uint32_t param2) {
		return GreaterEqual(param1, Formula::ConstantInt(param2));
	}
	template<Formula::IntFormula T>
	[[nodiscard]] constexpr auto operator>=(uint32_t param1, T param2) {
		return GreaterEqual(Formula::ConstantInt(param1), param2);
	}

	// And
	template<Formula::BoolFormula T, Formula::BoolFormula U>
	[[nodiscard]] constexpr auto operator&&(T param1, U param2) {
		return Formula::And(param1, param2);
	}
	template<Formula::BoolFormula T>
	[[nodiscard]] constexpr auto operator&&(T param1, bool param2) {
		return Formula::And(param1, Formula::ConstantBool(param2));
	}
	template<Formula::BoolFormula T>
	[[nodiscard]] constexpr auto operator&&(bool param1, T param2) {
		return Formula::And(Formula::ConstantBool(param1), param2);
	}

	// Or
	template<Formula::BoolFormula T, Formula::BoolFormula U>
	[[nodiscard]] constexpr auto operator||(T param1, U param2) {
		return Formula::Or(param1, param2);
	}
	template<Formula::BoolFormula T>
	[[nodiscard]] constexpr auto operator||(T param1, bool param2) {
		return Formula::Or(param1, Formula::ConstantBool(param2));
	}
	template<Formula::BoolFormula T>
	[[nodiscard]] constexpr auto operator||(bool param1, T param2) {
		return Formula::Or(Formula::ConstantBool(param1), param2);
	}
}// namespace Formula::Operators