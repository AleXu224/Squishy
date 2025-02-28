#pragma once

#include "Ui/elementToColor.hpp"
#include "color.hpp"
#include "formula/node.hpp"
#include "nodeData.hpp"
#include "string_view"
#include "utils/entryType.hpp"

namespace Node {
	struct Info {
		std::string_view name;
		squi::Color color = Utils::elementToColor(Misc::Element::physical);
		Utils::EntryType type = Utils::EntryType::points;
		bool optimizable = false;
		Formula::FloatNode formula;

		Formula::FloatNode _formula = formula;

		Data _data = InfoData{
			.type = type,
			.color = color,
			.optimizable = optimizable,
		};
	};
}// namespace Node