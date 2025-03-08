#pragma once

#include <optional>
#include <variant>

namespace Serialization::Save {
	struct BooleanOption {
		uint32_t hash;
		bool active;
	};

	struct ValueListOption {
		uint32_t hash{};
		std::optional<size_t> index;
	};

	using OptionTypes = std::variant<Serialization::Save::BooleanOption, Serialization::Save::ValueListOption>;
}// namespace Serialization::Save