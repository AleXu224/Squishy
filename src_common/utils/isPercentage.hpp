#pragma once

#include "format"
#include <stdexcept>
#include <typeinfo>

namespace Utils {
	template<class T>
	[[nodiscard]] constexpr bool isPercentage(const T &) {
		throw std::runtime_error(std::format("No isPercentage implemented for {}", typeid(T).name()));
		return false;
	}
}// namespace Utils