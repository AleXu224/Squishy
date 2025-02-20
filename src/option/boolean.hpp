#pragma once

#include "formula/node.hpp"
#include "string_view"
#include "utils/hashedString.hpp"
#include <optional>


namespace Option {
	struct Boolean {
		Utils::HashedString key;
		std::string_view name;
		std::optional<Formula::BoolNode> displayCondition = std::nullopt;
		bool active = false;

		void toggle() {
			active = !active;
		}
	};
}// namespace Option