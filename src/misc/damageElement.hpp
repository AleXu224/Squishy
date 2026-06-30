#pragma once

#include "cstdint"
#include "misc/element.hpp"
#include "string"
#include "utils/stringify.hpp"
#include <vector>

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
		allElemental,
	};

	static inline std::vector<DamageElement> damageElements{
		DamageElement::pyro,
		DamageElement::hydro,
		DamageElement::cryo,
		DamageElement::electro,
		DamageElement::dendro,
		DamageElement::anemo,
		DamageElement::geo,
		DamageElement::physical,
		DamageElement::all,
		DamageElement::allElemental,
	};

	[[nodiscard]] constexpr DamageElement damageElementFromElement(const Element &element) {
		switch (element) {
			case Element::pyro:
				return DamageElement::pyro;
			case Element::hydro:
				return DamageElement::hydro;
			case Element::cryo:
				return DamageElement::cryo;
			case Element::electro:
				return DamageElement::electro;
			case Element::dendro:
				return DamageElement::dendro;
			case Element::anemo:
				return DamageElement::anemo;
			case Element::geo:
				return DamageElement::geo;
			case Element::physical:
				return DamageElement::physical;
		}
		std::unreachable();
	}
}// namespace Misc

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
			case Misc::DamageElement::allElemental:
				return "All Elemental";
		}
		std::unreachable();
	}
}// namespace Utils