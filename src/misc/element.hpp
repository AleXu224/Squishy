#pragma once

#include "array"
#include "cstdint"
#include "utility"
#include "utils/stringify.hpp"


namespace Misc {
	enum class Element : uint8_t {
		pyro,
		hydro,
		cryo,
		electro,
		dendro,
		anemo,
		geo,
		physical,
	};

	static constexpr auto characterElements = std::array{
		Misc::Element::pyro,
		Misc::Element::hydro,
		Misc::Element::cryo,
		Misc::Element::electro,
		Misc::Element::dendro,
		Misc::Element::anemo,
		Misc::Element::geo,
	};

	static constexpr auto elements = std::array{
		Misc::Element::pyro,
		Misc::Element::hydro,
		Misc::Element::cryo,
		Misc::Element::electro,
		Misc::Element::dendro,
		Misc::Element::anemo,
		Misc::Element::geo,
		Misc::Element::physical,
	};

	static constexpr auto phecElements = std::array{
		Misc::Element::pyro,
		Misc::Element::hydro,
		Misc::Element::cryo,
		Misc::Element::electro,
	};

	template<class T>
	[[nodiscard]] inline constexpr auto ptrFromElement(const Misc::Element &element) {
		switch (element) {
			case Misc::Element::pyro:
				return &T::pyro;
			case Misc::Element::hydro:
				return &T::hydro;
			case Misc::Element::cryo:
				return &T::cryo;
			case Misc::Element::electro:
				return &T::electro;
			case Misc::Element::dendro:
				return &T::dendro;
			case Misc::Element::anemo:
				return &T::anemo;
			case Misc::Element::geo:
				return &T::geo;
			case Misc::Element::physical:
				return &T::physical;
		}
		std::unreachable();
	}
}// namespace Misc

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
