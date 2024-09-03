#pragma once

#include "fmt/core.h"
#include "string"


namespace Utils {
	template<class T>
	inline std::string Stringify(const T & /*unused*/) {
		return fmt::format("Cannot stringify {}", typeid(T).name());
	}
}// namespace Utils