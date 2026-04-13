#pragma once

#include "utility"

namespace Utils {
	// Why does this exist if there already is std::optional you might ask yourself
	// Well the answer is that msvc is stupid and std::optional fails in consteval for no good reason
	// while the same code works flawlessly on msvc and clang 🤷
	// https://godbolt.org/z/bb379jfxY

	template<class T>
	struct JankyOptional {
		constexpr JankyOptional() : _hasValue(false), _value() {}
		constexpr JankyOptional(T value) : _hasValue(true), _value(value) {}

		constexpr T value_or(this auto &&self, const T &other) {
			if (self._hasValue) return self._value;
			return other;
		}

		[[nodiscard]] constexpr bool has_value(this auto &&self) {
			return self._hasValue;
		}

		[[nodiscard]] constexpr T value(this auto &&self) {
			return self._value;
		}

		[[nodiscard]] constexpr const JankyOptional &this_or(this auto &&self, JankyOptional<T> &&other) {
			if (self._hasValue) return self;
			return std::move(other);
		}

	private:
		bool _hasValue;
		T _value;
	};
}// namespace Utils