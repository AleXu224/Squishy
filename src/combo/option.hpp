#pragma once

#include "character/key.hpp"
#include <optional>
#include <variant>


namespace Combo {
	struct ComboFloatOption {
		float value;
	};

	struct Option {
		Character::InstanceKey key{};
		uint32_t hash{};
		std::variant<bool, std::optional<uint8_t>, ComboFloatOption> value{};
	};
}// namespace Combo