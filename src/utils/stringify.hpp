#pragma once

#include "string"

namespace Utils {
	template<class T>
	inline std::string Stringify(const T & /*unused*/) {
		return "Cannot stringify";
	}
}// namespace Utils