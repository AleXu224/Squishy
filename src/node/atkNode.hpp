#pragma once

#include "formula/attackModifier.hpp"
#include "misc/attackSource.hpp"
#include "misc/element.hpp"
#include "nodeData.hpp"
#include "string_view"


namespace Node {
	struct Atk {
		std::string_view name;
		Utils::JankyOptional<Misc::Element> element{};
		Misc::AttackSource source{};
		Formula::FloatNode formula;
		Formula::Modifier modifier{};

		[[nodiscard]] static Formula::FloatNode _getFormula(
			const Utils::JankyOptional<Misc::Element> &element,
			Misc::AttackSource source,
			const Formula::FloatNode &formula,
			const Formula::Modifier &modifier
		);

		Formula::FloatNode _formula = _getFormula(element, source, formula, modifier);

		Data _data = AtkData{
			.element = element,
			.source = source,
		};
	};
}// namespace Node