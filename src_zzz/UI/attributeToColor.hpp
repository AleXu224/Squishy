#pragma once

#include "color.hpp"
#include "misc/attribute.hpp"
#include "utility"


namespace Utils {
	inline squi::Color attributeToColor(const Misc::Attribute &attribute) {
		switch (attribute) {
			case Misc::Attribute::physical:
				return squi::Color::rgb(240, 209, 42);
			case Misc::Attribute::fire:
				return squi::Color::rgb(255, 85, 34);
			case Misc::Attribute::ice:
				return squi::Color::rgb(152, 239, 240);
			case Misc::Attribute::electric:
				return squi::Color::rgb(46, 182, 255);
			case Misc::Attribute::ether:
				return squi::Color::rgb(254, 66, 126);
		}
		std::unreachable();
	}
	inline squi::Color attributeToBgColor(const Misc::Attribute &attribute) {
		return attributeToColor(attribute);
	}
}// namespace Utils