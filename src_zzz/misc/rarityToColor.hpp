#pragma once

#include "color.hpp"
#include "cstdint"
#include "unordered_map"


namespace Misc {
	static inline auto rarityToColor = std::unordered_map<uint8_t, squi::Color>{
		{1, squi::Color::rgb(125, 168, 155)},
		{2, squi::Color::rgb(125, 168, 155)},
		{3, squi::Color::rgb(0, 169, 255)},
		{4, squi::Color::rgb(233, 0, 255)},
		{5, squi::Color::rgb(255, 181, 0)},
	};
}