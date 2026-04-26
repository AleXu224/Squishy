#pragma once

#include "array"
#include "cstdint"
#include "utility"
#include "utils/stringify.hpp"


namespace Misc {
	enum class Attribute : uint8_t {
		physical,
		fire,
		ice,
		electric,
		ether,
	};

	static constexpr auto agentAttributes = std::array{
		Misc::Attribute::physical,
		Misc::Attribute::fire,
		Misc::Attribute::ice,
		Misc::Attribute::electric,
		Misc::Attribute::ether,
	};

	static constexpr auto attributes = std::array{
		Misc::Attribute::physical,
		Misc::Attribute::fire,
		Misc::Attribute::ice,
		Misc::Attribute::electric,
		Misc::Attribute::ether,
	};
}// namespace Misc

namespace Utils {
	template<>
	constexpr std::string Stringify<>(const Misc::Attribute &attribute) {
		switch (attribute) {
			case Misc::Attribute::physical:
				return "Physical";
			case Misc::Attribute::fire:
				return "Fire";
			case Misc::Attribute::ice:
				return "Ice";
			case Misc::Attribute::electric:
				return "Electric";
			case Misc::Attribute::ether:
				return "Ether";
		}
		std::unreachable();
	}
}// namespace Utils
