#pragma once

#include "color.hpp"
#include <unordered_map>

namespace Squishy {
	static std::unordered_map<uint8_t, squi::Color> qualityColors{
		{1, squi::Color::RGBA255(129, 132, 134)},
		{2, squi::Color::RGBA255(90, 151, 122)},
		{3, squi::Color::RGBA255(89, 135, 173)},
		{4, squi::Color::RGBA255(148, 112, 187)},
		{5, squi::Color::RGBA255(200, 124, 36)},
	};
}