#pragma once

#include "formula/attackModifier.hpp"
#include "nodeData.hpp"


namespace Node {
	struct PolarityDisorder {
		Formula::NodeType<std::string> name;
		Formula::NodeType<Misc::Attribute> attribute;
		Formula::FloatNode multiplier;
		size_t index;
		Formula::Modifier modifier{};

		[[nodiscard]] static Formula::FloatNode _getFormula(
			const Formula::NodeType<Misc::Attribute> &attribute,
			const Formula::FloatNode &multiplier,
			size_t index,
			const Formula::Modifier &modifier
		);

		Formula::FloatNode _formula = _getFormula(attribute, multiplier, index, modifier);

		Data _data = AbloomData{
			.name = name,
			.attribute = attribute,
		};
	};
}// namespace Node