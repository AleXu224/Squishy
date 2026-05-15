#pragma once

#include "formula/attackModifier.hpp"
#include "misc/attribute.hpp"
#include "nodeData.hpp"


namespace Node {
	struct AbloomNode {
		Formula::NodeType<std::string> name;
		Misc::Attribute attribute;
		float multiplier;
		size_t index;
		Formula::Modifier modifier{};

		[[nodiscard]] static Formula::FloatNode _getFormula(
			const Misc::Attribute &attribute,
			float multiplier,
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