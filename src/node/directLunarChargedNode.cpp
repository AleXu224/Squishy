#include "directLunarChargedNode.hpp"

#include <utility>
#include "formula/clamp.hpp"
#include "formula/enemy.hpp"
#include "formula/operators.hpp"
#include "misc/element.hpp"
#include "modifiers/total/total.hpp"


namespace Node {
	using namespace Formula::Operators;

	Formula::FloatNode DirectLunarCharged::_getFormula(
		Misc::Element element,
		Formula::FloatNode formula,
		const Formula::Modifier &modifier
	) {
		auto totalDMG = modifier.DMG + Modifiers::total().lunarCharged.DMG;
		auto totalMultiplicativeDMG = modifier.multiplicativeDMG + Modifiers::total().lunarCharged.multiplicativeDMG;
		auto totalElevation = modifier.elevation + Modifiers::total().lunarCharged.elevation;
		auto totalCritRate = Formula::Clamp(modifier.critRate + Modifiers::total().lunarCharged.critRate + Modifiers::total().cr, 0.f, 1.f);
		auto totalCritDMG = modifier.critDMG + Modifiers::total().lunarCharged.critDMG + Modifiers::total().cd;

		auto multiplier = (1.0f + totalMultiplicativeDMG) * std::move(formula);
		auto elevation = 1.0f + totalElevation;
		auto emBonus = (6.f * Modifiers::total().em) / (Modifiers::total().em + Formula::ConstantFlat(2000.f));
		auto dmgBonus = (1.0f + emBonus + totalDMG);
		auto crit = 1.0f + totalCritRate * totalCritDMG;
		auto enemy = Formula::EnemyResMultiplier(Misc::AttackSource::burst, element);

		return multiplier
			 * elevation
			 * dmgBonus
			 * crit
			 * enemy
			 * 3.f;
	}

}// namespace Node