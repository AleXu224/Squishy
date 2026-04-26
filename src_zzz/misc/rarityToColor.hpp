#pragma once

#include "color.hpp"
#include "cstdint"
#include "unordered_map"


namespace Misc {
	static inline auto rarityToColor = std::unordered_map<uint8_t, squi::Color>{
		{1, squi::Color::css(0x72768b, 1.f)},
		{2, squi::Color::css(0x2b8f72, 1.f)},
		{3, squi::Color::css(0x517fcb, 1.f)},
		{4, squi::Color::css(0xa156e0, 1.f)},
		{5, squi::Color::css(0xbc6932, 1.f)},
	};
}