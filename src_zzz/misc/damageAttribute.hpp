#pragma once

#include "cstdint"
#include "misc/attribute.hpp"
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
		wind,
		all,
	};

	static inline std::vector<DamageAttribute> damageAttributes{
		DamageAttribute::physical,
		DamageAttribute::fire,
		DamageAttribute::ice,
		DamageAttribute::electric,
		DamageAttribute::ether,
		DamageAttribute::wind,
		DamageAttribute::all,
	};

	[[nodiscard]] constexpr DamageAttribute damageAttributeFromAttribute(const Attribute &attribute) {
		switch (attribute) {
			case Attribute::physical:
				return DamageAttribute::physical;
			case Attribute::fire:
				return DamageAttribute::fire;
			case Attribute::ice:
				return DamageAttribute::ice;
			case Attribute::electric:
				return DamageAttribute::electric;
			case Attribute::ether:
				return DamageAttribute::ether;
			case Attribute::wind:
				return DamageAttribute::wind;
		}
		std::unreachable();
	}
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
			case Misc::DamageAttribute::wind:
				return "Wind";
			case Misc::DamageAttribute::all:
				return "All";
		}
		std::unreachable();
	}
}// namespace Utils