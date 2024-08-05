#pragma once

#include "string_view"
#include <optional>
#include <vector>

namespace Conditional {
	struct ValueList {
		std::string_view key;
		std::string_view prefix;
		std::optional<size_t> currentIndex = std::nullopt;
		const std::vector<uint32_t> values;

		std::optional<uint32_t> getValue() const {
			if (!currentIndex.has_value()) return std::nullopt;
			return values.at(currentIndex.value());
		}
	};
}// namespace Conditional