#pragma once

#include "UI/elementToColor.hpp"
#include "color.hpp"
#include "formula/node.hpp"
#include "nodeData.hpp"
#include "utils/entryType.hpp"

namespace Node {
	struct Info {
		std::string name;
		squi::Color color = Utils::elementToColor(Misc::Element::physical);
		Utils::EntryType type = Utils::EntryType::points;
		bool optimizable = false;
		Formula::FloatNode formula;

		Formula::FloatNode _formula = formula;

		Data _data = InfoData{
			.name = name,
			.type = type,
			.color = color,
			.optimizable = optimizable,
		};
	};
}// namespace Node