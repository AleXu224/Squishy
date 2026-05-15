#pragma once

#include "formula/constant.hpp"
#include "formula/node.hpp"
#include "nodeData.hpp"
#include "stats/sheet.hpp"

namespace Node {
	struct Mods {
		Stats::ModsSheet mods{};
		std::string name = "Mods Display";
		Formula::FloatNode _formula = Formula::ConstantFlat({}, 0);

		Data _data = ModsData{
			.name = name,
			.mods = mods,
		};
	};
}// namespace Node