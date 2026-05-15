#pragma once

#include "formula/customAttackModifier.hpp"
#include "misc/element.hpp"
#include "nodeData.hpp"


namespace Node {
	struct CustomAtk {
		std::string name;
		Misc::Element element = Misc::Element::physical;
		Formula::FloatNode formula;
		Formula::CustomAtkModifier modifier{};

		[[nodiscard]] static Formula::FloatNode _getFormula(
			Misc::Element element,
			Formula::FloatNode formula,
			Formula::CustomAtkModifier modifier
		);

		Formula::FloatNode _formula = _getFormula(element, formula, modifier);

		Data _data = AtkData{
			.name = name,
			.element = element,
		};
	};
}// namespace Node