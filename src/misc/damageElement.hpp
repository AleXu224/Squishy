#pragma once

#include "cstdint"
#include "string"
#include "utils/stringify.hpp"

namespace Misc {
	enum class DamageElement : uint8_t {
		pyro,
		hydro,
		cryo,
		electro,
		dendro,
		anemo,
		geo,
		physical,
		all,
	};
}

namespace Utils {
	template<>
	constexpr std::string Stringify<>(const Misc::DamageElement &element) {
		switch (element) {
			case Misc::DamageElement::pyro:
				return "Pyro";
			case Misc::DamageElement::hydro:
				return "Hydro";
			case Misc::DamageElement::cryo:
				return "Cryo";
			case Misc::DamageElement::electro:
				return "Electro";
			case Misc::DamageElement::dendro:
				return "Dendro";
			case Misc::DamageElement::anemo:
				return "Anemo";
			case Misc::DamageElement::geo:
				return "Geo";
			case Misc::DamageElement::physical:
				return "Physical";
			case Misc::DamageElement::all:
				return "All";
		}
		std::unreachable();
	}
}// namespace Utils