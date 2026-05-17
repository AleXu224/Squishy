#pragma once

#include "format"
#include "string"
#include "typeinfo"// IWYU pragma: keep

namespace Utils {
	template<class T>
	inline std::string Stringify(const T & /*unused*/) {
		return std::format("Cannot stringify {}", typeid(T).name());
	}

	inline std::string formatFloat(float value) {
		if (value > 1000.f) {
			return std::format("{:.0f}", value);
		}
		std::string str = std::format("{:.2f}", value);

		if (str.find('.') != std::string::npos) {
			str.erase(str.find_last_not_of('0') + 1, std::string::npos);
			if (str.back() == '.') {
				str.pop_back();
			}
		}
		return str;
	}
}// namespace Utils