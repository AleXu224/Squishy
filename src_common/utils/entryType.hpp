#pragma once

#include "format"
#include "formula/percentage.hpp"
#include <utility>

namespace Utils {
	enum class EntryType {
		multiplier,
		seconds,
		energy,
		points,
	};

	[[nodiscard]] static inline std::string printEntryType(float value, EntryType type) {
		switch (type) {
			case EntryType::multiplier:
				return Formula::Percentage({}, value, true);
			case EntryType::seconds:
				return std::format("{}s", Utils::formatFloat(value));
			case EntryType::points:
			case EntryType::energy:
				return std::format("{}", Utils::formatFloat(value));
		}
		std::unreachable();
	}
}// namespace Utils
