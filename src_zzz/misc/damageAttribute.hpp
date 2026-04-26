#pragma once

#include "cstdint"
#include "string"
#include "utils/stringify.hpp"
#include <vector>

namespace Misc {
	enum class DamageAttribute : uint8_t {
		physical,
		fire,
		ice,
		electric,
		ether,
		all,
	};

	static inline std::vector<DamageAttribute> damageAttributes{
		DamageAttribute::physical,
		DamageAttribute::fire,
		DamageAttribute::ice,
		DamageAttribute::electric,
		DamageAttribute::ether,
		DamageAttribute::all,
	};
}// namespace Misc

namespace Utils {
	template<>
	constexpr std::string Stringify<>(const Misc::DamageAttribute &element) {
		switch (element) {
			case Misc::DamageAttribute::physical:
				return "Physical";
			case Misc::DamageAttribute::fire:
				return "Fire";
			case Misc::DamageAttribute::ice:
				return "Ice";
			case Misc::DamageAttribute::electric:
				return "Electric";
			case Misc::DamageAttribute::ether:
				return "Ether";
			case Misc::DamageAttribute::all:
				return "All";
		}
		std::unreachable();
	}
}// namespace Utils