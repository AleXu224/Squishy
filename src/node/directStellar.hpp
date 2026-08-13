#pragma once

#include "formula/attackModifier.hpp"
#include "misc/stellarDamageType.hpp"
#include "nodeData.hpp"


namespace Node {
	struct DirectStellar {
		std::string name;
		Misc::StellarDamageType damageType = Misc::StellarDamageType::stellarSwirl;
		Formula::FloatNode formula;
		Formula::Modifier modifier{};

		[[nodiscard]] static Formula::FloatNode _getFormula(
			Misc::StellarDamageType damageType,
			const Formula::FloatNode &formula,
			const Formula::Modifier &modifier
		);

		Formula::FloatNode _formula = _getFormula(damageType, formula, modifier);

		Data _data = DirectStellarData{
			.name = name,
			.damageType = damageType,
		};
	};
}// namespace Node