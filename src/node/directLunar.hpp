#pragma once

#include "formula/attackModifier.hpp"
#include "misc/element.hpp"
#include "misc/lunarDamageType.hpp"
#include "nodeData.hpp"
#include "string_view"


namespace Node {
	struct DirectLunar {
		std::string_view name;
		Misc::LunarDamageType damageType = Misc::LunarDamageType::lunarCharged;
		Formula::FloatNode formula;
		Formula::Modifier modifier{};

		[[nodiscard]] static Formula::FloatNode _getFormula(
			Misc::LunarDamageType damageType,
			const Formula::FloatNode &formula,
			const Formula::Modifier &modifier
		);

		Formula::FloatNode _formula = _getFormula(damageType, formula, modifier);

		Data _data = DirectLunarData{
			.damageType = damageType,
		};
	};
}// namespace Node