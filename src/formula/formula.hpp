#pragma once

#include "conditional.hpp"// IWYU pragma: export
#include "constant.hpp"
#include "custom.hpp"// IWYU pragma: export
#include "product.hpp"
#include "refinement.hpp"// IWYU pragma: export
#include "requires.hpp"  // IWYU pragma: export
#include "stat.hpp"      // IWYU pragma: export
#include "sum.hpp"


namespace Formula {
	template<IntermediaryLike T, IntermediaryLike U>
	[[nodiscard]] consteval auto operator+(T param1, U param2) {
		return Sum(param1, param2);
	}

	template<IntermediaryLike T>
	[[nodiscard]] consteval auto operator+(float param1, T param2) {
		return Sum(Constant(param1), param2);
	}

	template<IntermediaryLike T>
	[[nodiscard]] consteval auto operator+(T param1, float param2) {
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