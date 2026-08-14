#include "directStellar.hpp"

#include "formula/clamp.hpp"
#include "formula/enemy.hpp"
#include "formula/nodes.hpp"
#include "formula/operators.hpp"
#include "formula/option.hpp"
#include "modifiers/total/total.hpp"
#include <utility>


namespace Node {
	using namespace Formula::Operators;

	[[nodiscard]] static Formula::FloatNode getDirectStellarMultiplier(Misc::StellarDamageType damageType) {
		switch (damageType) {
			case Misc::StellarDamageType::stellarConduct: {
				auto stacks = Formula::impl_GetFloatPassive{.name = "stellarConductCoefficientStacks"};
				return Formula::IfElse{
					.requirement = stacks >= 1.f,
					.trueVal = 1.4f + stacks * 0.05f,
					.elseVal = Formula::Constant{.value = 1.0f},
				};
			}
			case Misc::StellarDamageType::stellarSwirl:
				return Formula::Constant{.value = 1.0f};
		}
		std::unreachable();
	}

	[[nodiscard]] constexpr auto _getTotalEnemy(
		Utils::JankyOptional<Misc::Element> element,
		Misc::StellarDamageType damageType,
		const auto &formula
	) {
		Formula::EnemyModifier modifiers{};
		modifiers = modifiers + Modifiers::total().all.enemy
				  + Formula::getElementModifierSimpleOptional(element)
				  + Formula::getStellarDamageModifier(element, damageType)
				  + formula;

		return modifiers;
	}

	Formula::FloatNode DirectStellar::_getFormula(
		Utils::JankyOptional<Misc::Element> element,
		Misc::StellarDamageType damageType,
		const Formula::FloatNode &formula,
		const Formula::Modifier &modifier
	) {
		auto totalDMG = modifier.DMG + Formula::NodeDirectStellar{.element = element, .damageType = damageType, .skillStat = Misc::SkillStat::DMG} + Modifiers::total().allStellar.DMG;
		auto totalMultiplicativeDMG = modifier.multiplicativeDMG + Formula::NodeDirectStellar{.element = element, .damageType = damageType, .skillStat = Misc::SkillStat::multiplicativeDMG} + Modifiers::total().allStellar.multiplicativeDMG;
		auto totalAdditiveDMG = modifier.additiveDMG + Formula::NodeDirectStellar{.element = element, .damageType = damageType, .skillStat = Misc::SkillStat::additiveDMG} + Modifiers::total().allStellar.additiveDMG;
		auto totalElevation = modifier.elevation + Formula::NodeDirectStellar{.element = element, .damageType = damageType, .skillStat = Misc::SkillStat::elevation} + Modifiers::total().allStellar.elevation;
		auto totalCritRate = Formula::Clamp({}, modifier.critRate + Formula::NodeDirectStellar{.element = element, .damageType = damageType, .skillStat = Misc::SkillStat::critRate} + Modifiers::total().allStellar.critRate + Modifiers::total().cr + Formula::NodeElementSimpleOptional{.element = element, .skillStat = Misc::SkillStat::critRate}, 0.f, 1.f);
		auto totalCritDMG = modifier.critDMG + Formula::NodeDirectStellar{.element = element, .damageType = damageType, .skillStat = Misc::SkillStat::critDMG} + Modifiers::total().allStellar.critDMG + Modifiers::total().cd + Formula::NodeElementSimpleOptional{.element = element, .skillStat = Misc::SkillStat::critDMG};

		auto multiplier = (1.0f + totalMultiplicativeDMG) * formula;
		auto emBonus = (6.f * Modifiers::total().em) / (Modifiers::total().em + Formula::ConstantFlat({}, 2000.f));
		auto dmgBonus = (1.0f + emBonus + totalDMG);
		auto crit = 1.0f + totalCritRate * totalCritDMG;
		auto totalModifier = _getTotalEnemy(element, damageType, modifier.enemy);
		auto enemy = Formula::EnemyResMultiplier{.attackSource = Misc::AttackSource::burst, .element = element, .modifiers = totalModifier.resistance};
		auto elevation = 1.0f + totalElevation;

		auto baseMultiplier = getDirectStellarMultiplier(damageType);

		return (multiplier * dmgBonus * baseMultiplier
				+ totalAdditiveDMG)
			 * crit
			 * enemy
			 * elevation;
	}

}// namespace Node