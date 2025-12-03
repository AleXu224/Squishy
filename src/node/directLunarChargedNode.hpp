#pragma once

#include "formula/attackModifier.hpp"
#include "misc/element.hpp"
#include "nodeData.hpp"
#include "string_view"


namespace Node {
	struct DirectLunarCharged {
		std::string_view name;
		Misc::Element element = Misc::Element::electro;
		Formula::FloatNode formula;
		Formula::Modifier modifier{};

		[[nodiscard]] static Formula::FloatNode _getFormula(
			Misc::Element element,
			Formula::FloatNode formula,
			const Formula::Modifier &modifier
		);

		Formula::FloatNode _formula = _getFormula(element, formula, modifier);

		Data _data = CustomAtkData{
			.element = element,
		};
	};
}// namespace Node