#pragma once

#include "formula/attackModifier.hpp"
#include "misc/attackSource.hpp"
#include "misc/attribute.hpp"
#include "nodeData.hpp"


namespace Node {
	struct Daze {
		std::string name;
		Utils::JankyOptional<Misc::Attribute> attribute{};
		Utils::JankyOptional<Misc::AttackSource> source{};
		Formula::FloatNode formula;
		Formula::Modifier modifier{};

		[[nodiscard]] static Formula::FloatNode _getFormula(
			const Utils::JankyOptional<Misc::Attribute> &attribute,
			const Utils::JankyOptional<Misc::AttackSource> &source,
			const Formula::FloatNode &formula,
			const Formula::Modifier &modifier
		);

		Formula::FloatNode _formula = _getFormula(attribute, source, formula, modifier);

		Data _data = DazeData{
			.name = name,
			.attribute = attribute,
			.source = source,
		};
	};
}// namespace Node