#pragma once

#include "utils/stringify.hpp"

namespace Misc {
	enum class Element {
		pyro,
		hydro,
		cryo,
		electro,
		dendro,
		anemo,
		geo,
		physical,
	};
}

namespace Utils {
	template<>
	constexpr std::string Stringify<>(const Misc::Element &element) {
		switch (element) {
			case Misc::Element::pyro:
				return "Pyro";
			case Misc::Element::hydro:
				return "Hydro";
			case Misc::Element::cryo:
				return "Cryo";
			case Misc::Element::electro:
				return "Electro";
			case Misc::Element::dendro:
				return "Dendro";
			case Misc::Element::anemo:
				return "Anemo";
			case Misc::Element::geo:
				return "Geo";
			case Misc::Element::physical:
				return "Physical";
		}
		std::unreachable();
	}
}// namespace Utils
