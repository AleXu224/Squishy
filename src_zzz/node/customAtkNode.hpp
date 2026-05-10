#pragma once

#include "formula/attackModifier.hpp"
#include "misc/attribute.hpp"
#include "nodeData.hpp"
#include "string_view"


namespace Node {
	struct CustomAtk {
		std::string_view name;
		Misc::Attribute attribute;
		Formula::FloatNode formula;
		Formula::Modifier modifier{};

		[[nodiscard]] static Formula::FloatNode _getFormula(
			const Misc::Attribute &attribute,
			const Formula::FloatNode &formula,
			const Formula::Modifier &modifier
		);

		Formula::FloatNode _formula = _getFormula(attribute, formula, modifier);

		Data _data = CustomAtkData{
			.attribute = attribute,
		};
	};
}// namespace Node