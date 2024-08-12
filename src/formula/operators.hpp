#pragma once

#include "constant.hpp"
#include "product.hpp"
#include "sum.hpp"

namespace Formula {
	template<IntermediaryLike T, IntermediaryLike U>
	[[nodiscard]] consteval auto operator+(const T &param1, const U &param2) {
		return Sum(param1, param2);
	}

	template<IntermediaryLike T>
	[[nodiscard]] consteval auto operator+(const float &param1, const T &param2) {
		return Sum(Constant(param1), param2);
	}

	template<IntermediaryLike T>
	[[nodiscard]] consteval auto operator+(const T &param1, const float &param2) {
		return Sum(param1, Constant(param2));
	}

	template<IntermediaryLike T, IntermediaryLike U>
	[[nodiscard]] consteval auto operator*(T param1, U param2) {
		return Product(param1, param2);
	}

	template<IntermediaryLike T>
	[[nodiscard]] consteval auto operator*(float param1, T param2) {
		return Product(Constant(param1), param2);
	}

	template<IntermediaryLike T>
	[[nodiscard]] consteval auto operator*(T param1, float param2) {
		return Product(param1, Constant(param2));
	}
}// namespace Formula