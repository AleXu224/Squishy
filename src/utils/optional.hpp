#pragma once

namespace Utils {
	// Why does this exist if there already is std::optional you might ask yourself
	// Well the answer is that msvc is stupid and std::optional fails in consteval for no good reason
	// while the same code works flawlessly on msvc and clang 🤷
	// https://godbolt.org/z/bb379jfxY

	template<class T>
	struct JankyOptional {
		constexpr JankyOptional() : hasValue(false), value() {}
		constexpr JankyOptional(const T &value) : hasValue(true), value(value) {}

		constexpr T value_or(this auto &&self, const T &other) {
			if (self.hasValue) return self.value;
			return other;
		}

	private:
		bool hasValue;
		T value;
	};
}// namespace Utils