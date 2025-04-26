#pragma once

#include "formula/healModifier.hpp"
#include "nodeData.hpp"
#include "string_view"


namespace Node {
	struct Heal {
		std::string_view name;
		Formula::FloatNode formula;
		Formula::HealModifier modifier{};

		[[nodiscard]] static Formula::FloatNode _getFormula(
			Formula::FloatNode formula,
			Formula::HealModifier modifier
		);

		Formula::FloatNode _formula = _getFormula(formula, modifier);

		Data _data = HealData{};
	};
}// namespace Node