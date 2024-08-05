#pragma once

#include "misc/element.hpp"
#include "color.hpp"


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
	}
}// namespace Utils