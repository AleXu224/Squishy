#pragma once

#include "fmt/core.h"
#include "string"
#include "typeinfo"// IWYU pragma: keep

namespace Utils {
	template<class T>
	inline std::string Stringify(const T & /*unused*/) {
		return fmt::format("Cannot stringify {}", typeid(T).name());
	}
}// namespace Utils