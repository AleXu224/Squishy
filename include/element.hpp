#pragma once
#include "color.hpp"
#include "cstdint"
#include <unordered_map>


namespace Squishy {
	enum class Element : uint8_t {
		Anemo = 0,
		Cryo = 1,
		Dendro = 2,
		Electro = 3,
		Geo = 4,
		Hydro = 5,
		Pyro = 6,
	};

	enum class DMGElement : uint8_t {
		Anemo = 0,
		Cryo = 1,
		Dendro = 2,
		Electro = 3,
		Geo = 4,
		Hydro = 5,
		Pyro = 6,
		Physical = 7,
	};

	static const std::unordered_map<DMGElement, squi::Color> elementColors{
		{DMGElement::Anemo, squi::Color::HEX(0x33ccb3FF)},
		{DMGElement::Cryo, squi::Color::HEX(0x98c8e8FF)},
		{DMGElement::Dendro, squi::Color::HEX(0x7bb42dFF)},
		{DMGElement::Electro, squi::Color::HEX(0xd376f0FF)},
		{DMGElement::Geo, squi::Color::HEX(0xcfa726FF)},
		{DMGElement::Hydro, squi::Color::HEX(0x1c72fdFF)},
		{DMGElement::Pyro, squi::Color::HEX(0xe2311dFF)},
        {DMGElement::Physical, squi::Color::HEX(0xAAAAAAFF)},
	};


}// namespace Squishy