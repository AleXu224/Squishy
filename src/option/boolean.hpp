#pragma once

#include "formula/node.hpp"
#include "string_view"
#include "utils/hashedString.hpp"
#include <optional>


namespace Option {
	struct Boolean {
		Utils::HashedString key;
		std::string_view name;
		bool teamBuff = false;
		std::optional<Formula::BoolNode> displayCondition = std::nullopt;
		bool active = false;

		void toggle() {
			active = !active;
		}
	};
}// namespace Option