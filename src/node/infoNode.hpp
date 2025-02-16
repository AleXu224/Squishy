#pragma once

#include "Ui/elementToColor.hpp"
#include "color.hpp"
#include "formula/node.hpp"
#include "nodeData.hpp"
#include "string_view"


namespace Node {
	struct Info {
		std::string_view name;
		squi::Color color = Utils::elementToColor(Misc::Element::physical);
		bool isPercentage = false;
		Formula::FloatNode formula;

		Formula::FloatNode _formula = formula;

		Data _data = InfoData{
			.isPercentage = isPercentage,
			.color = color,
		};
	};
}// namespace Node