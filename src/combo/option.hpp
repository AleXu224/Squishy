#pragma once

#include "character/key.hpp"
#include <optional>
#include <variant>


namespace Combo {
	struct Option {
		Character::InstanceKey key;
		uint32_t hash;
		std::variant<bool, std::optional<uint8_t>> value;
	};
}// namespace Combo