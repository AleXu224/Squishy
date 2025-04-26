#pragma once

#include "color.hpp"
#include "misc/element.hpp"
#include "utility"


namespace Utils {
	inline squi::Color elementToColor(const Misc::Element &element) {
		switch (element) {
			case Misc::Element::anemo:
				return 0x33ccb3FF;
			case Misc::Element::geo:
				return 0xcfa726FF;
			case Misc::Element::electro:
				return 0xd376f0ff;
			case Misc::Element::dendro:
				return 0x7bb42dff;
			case Misc::Element::hydro:
				return 0x1c72fdff;
			case Misc::Element::pyro:
				return 0xe2311dff;
			case Misc::Element::cryo:
				return 0x98c8e8ff;
			case Misc::Element::physical:
				return 0xffffffff;
		}
		std::unreachable();
	}
	inline squi::Color elementToBgColor(const Misc::Element &element) {
		switch (element) {
			case Misc::Element::anemo:
				return squi::Color::css(14, 43, 46);
			case Misc::Element::geo:
				return squi::Color::css(42, 34, 20);
			case Misc::Element::electro:
				return squi::Color::css(32, 20, 55);
			case Misc::Element::dendro:
				return squi::Color::css(26, 42, 26);
			case Misc::Element::hydro:
				return squi::Color::css(24, 33, 69);
			case Misc::Element::pyro:
				return squi::Color::css(44, 19, 19);
			case Misc::Element::cryo:
				return squi::Color::css(12, 39, 65);
			case Misc::Element::physical:
				return 0xffffffff;
		}
		std::unreachable();
	}
}// namespace Utils