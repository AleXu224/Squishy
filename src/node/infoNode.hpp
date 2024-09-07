#pragma once

#include "Ui/elementToColor.hpp"
#include "color.hpp"
#include "nodeData.hpp"
#include "string_view"


namespace Node {
	template<class Frm>
	struct Info {
		std::string_view name;
		squi::Color color = Utils::elementToColor(Misc::Element::physical);
		bool isPercentage = false;
		Frm formula;

		Frm _formula = formula;

		Data _data = InfoData{
			.isPercentage = isPercentage,
			.color = color,
		};
	};
}// namespace Node