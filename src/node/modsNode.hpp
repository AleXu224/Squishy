#pragma once

#include "formula/constant.hpp"
#include "formula/node.hpp"
#include "nodeData.hpp"
#include "stats/sheet.hpp"
#include "string_view"

namespace Node {
	struct Mods {
		Stats::ModsSheet mods{};
		std::string_view name = "Mods Display";
		Formula::FloatNode _formula = Formula::ConstantFlat(0);

		Data _data = ModsData{
			.mods = mods,
		};
	};
}// namespace Node