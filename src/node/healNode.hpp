#pragma once

#include "formula/healModifier.hpp"
#include "nodeData.hpp"


namespace Node {
	struct Heal {
		std::string name;
		Formula::FloatNode formula;
		Formula::HealModifier modifier{};

		[[nodiscard]] static Formula::FloatNode _getFormula(
			Formula::FloatNode formula,
			Formula::HealModifier modifier
		);

		Formula::FloatNode _formula = _getFormula(formula, modifier);

		Data _data = HealData{
			.name = name,
		};
	};
}// namespace Node