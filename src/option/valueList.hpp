#pragma once

#include "formula/node.hpp"
#include "node/node.hpp"
#include "stats/sheet.hpp"
#include "string_view"
#include "utils/hashedString.hpp"
#include <cstdint>
#include <optional>
#include <vector>


namespace Option {
	struct ValueList {
		Utils::HashedString key;
		std::string_view prefix;
		bool teamBuff = false;
		std::optional<size_t> currentIndex = std::nullopt;
		std::optional<Formula::BoolNode> displayCondition = std::nullopt;
		std::vector<uint32_t> values;
		Stats::ModsSheet mods{};
		std::vector<Node::Types> nodes{};

		[[nodiscard]] std::optional<uint32_t> getValue() const {
			if (!currentIndex.has_value()) return std::nullopt;
			return values.at(currentIndex.value());
		}
	};
}// namespace Option
