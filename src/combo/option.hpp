#pragma once

#include "character/key.hpp"
#include <optional>
#include <variant>


namespace Combo {
	struct Option {
		Character::InstanceKey key;
		std::variant<bool, std::optional<size_t>> value;
	};
}// namespace Combo