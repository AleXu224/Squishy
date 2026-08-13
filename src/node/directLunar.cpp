#include "directLunar.hpp"

#include "formula/clamp.hpp"
#include "formula/enemy.hpp"
#include "formula/nodes.hpp"
#include "formula/operators.hpp"
#include "modifiers/total/total.hpp"
#include <utility>


namespace Node {
	using namespace Formula::Operators;

	[[nodiscard]] static float getDirectLunarMultiplier(Misc::LunarDamageType damageType) {
		switch (damageType) {
			case Misc::LunarDamageType::lunarCharged:
				return 3.f;
			case Misc::LunarDamageType::lunarBloom:
				return 1.f;
			case Misc::LunarDamageType::lunarCrystallize:
				return 1.6f;
		}
		std::unreachable();
	}

	[[nodiscard]] constexpr auto _getTotalEnemy(
		Misc::LunarDamageType damageType,
		const auto &formula
	) {
		Formula::EnemyModifier modifiers{};
		modifiers = modifiers + Modifiers::total().all.enemy
				  + Formula::getElementModifierSimple(Misc::lunarDamageTypeToElement(damageType))
				  + Formula::getLunarDamageModifier(damageType)
				  + formula;

		return modifiers;
	}

	Formula::FloatNode DirectLunar::_getFormula(
		Misc::LunarDamageType damageType,
		const Formula::FloatNode &formula,
		const Formula::Modifier &modifier
	) {
		auto totalDMG = modifier.DMG + Formula::NodeDirectLunar{.damageType = damageType, .skillStat = Misc::SkillStat::DMG} + Modifiers::total().allLunar.DMG;
		auto totalMultiplicativeDMG = modifier.multiplicativeDMG + Formula::NodeDirectLunar{.damageType = damageType, .skillStat = Misc::SkillStat::multiplicativeDMG} + Modifiers::total().allLunar.multiplicativeDMG;
		auto totalAdditiveDMG = modifier.additiveDMG + Formula::NodeDirectLunar{.damageType = damageType, .skillStat = Misc::SkillStat::additiveDMG} + Modifiers::total().allLunar.additiveDMG;
		auto totalElevation = modifier.elevation + Formula::NodeDirectLunar{.damageType = damageType, .skillStat = Misc::SkillStat::elevation} + Modifiers::total().allLunar.elevation;
		auto totalCritRate = Formula::Clamp({}, modifier.critRate + Formula::NodeDirectLunar{.damageType = damageType, .skillStat = Misc::SkillStat::critRate} + Modifiers::total().allLunar.critRate + Modifiers::total().cr + Stats::fromElement(Modifiers::total(), Misc::lunarDamageTypeToElement(damageType)).critRate, 0.f, 1.f);
		auto totalCritDMG = modifier.critDMG + Formula::NodeDirectLunar{.damageType = damageType, .skillStat = Misc::SkillStat::critDMG} + Modifiers::total().allLunar.critDMG + Modifiers::total().cd + Stats::fromElement(Modifiers::total(), Misc::lunarDamageTypeToElement(damageType)).critDMG;

		auto multiplier = (1.0f + totalMultiplicativeDMG) * formula;
		auto emBonus = (6.f * Modifiers::total().em) / (Modifiers::total().em + Formula::ConstantFlat({}, 2000.f));
		auto dmgBonus = (1.0f + emBonus + totalDMG);
		auto crit = 1.0f + totalCritRate * totalCritDMG;
		auto totalModifier = _getTotalEnemy(damageType, modifier.enemy);
		auto enemy = Formula::EnemyResMultiplier{.attackSource = {}, .element = Misc::lunarDamageTypeToElement(damageType), .modifiers = totalModifier.resistance};
		auto elevation = 1.0f + totalElevation;

		auto finalMultiplier = getDirectLunarMultiplier(damageType);

		return (multiplier * dmgBonus * finalMultiplier
				+ totalAdditiveDMG)
			 * crit
			 * enemy
			 * elevation;
	}

}// namespace Node