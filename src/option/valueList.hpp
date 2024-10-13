#pragma once

#include "string_view"
#include "utils/hashedString.hpp"
#include <cstdint>
#include <optional>
#include <vector>


namespace Option {
	struct ValueList {
		Utils::HashedString key;
		std::string_view prefix;
		std::optional<size_t> currentIndex = std::nullopt;
		std::vector<uint32_t> values;

		[[nodiscard]] std::optional<uint32_t> getValue() const {
			if (!currentIndex.has_value()) return std::nullopt;
			return values.at(currentIndex.value());
		}
	};
}// namespace Option
