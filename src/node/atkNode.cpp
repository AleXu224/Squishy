#include "atkNode.hpp"
#include "formula/nodes.hpp"
#include "formula/operators.hpp"
#include "formula/reaction.hpp"
#include "misc/element.hpp"
#include "misc/skillStat.hpp"
#include "modifiers/total/total.hpp"



namespace Node {
	using namespace Formula::Operators;
	template<Misc::SkillStat skillStat>
	[[nodiscard]] static constexpr auto _getTotal(
		Utils::JankyOptional<Misc::Element> attackElement,
		const Utils::JankyOptional<Misc::AttackSource> &atkSource,
		const auto &formula
	) {
		auto allStats = Stats::fromSkillStat(Modifiers::total().all, skillStat);
		auto elementStats = Formula::NodeElement({}, attackElement, atkSource, skillStat);
		auto skillStats = Formula::NodeSkill({}, atkSource, skillStat);

		return allStats + elementStats + skillStats + formula;
	}

	[[nodiscard]] constexpr auto _getTotalEnemy(
		Utils::JankyOptional<Misc::Element> attackElement,
		const Utils::JankyOptional<Misc::AttackSource> &atkSource,
		const auto &formula
	) {
		Formula::EnemyModifier modifiers{};
		modifiers = modifiers + Modifiers::total().all.enemy
				  + Formula::getElementModifier(atkSource, attackElement)
				  + Formula::getSourceModifier(atkSource)
				  + formula;

		return modifiers;
	}

	Formula::FloatNode Atk::_getFormula(
		const Utils::JankyOptional<Misc::Element> &element,
		const Utils::JankyOptional<Misc::AttackSource> &source,
		const Formula::FloatNode &formula,
		const Formula::Modifier &modifier
	) {
		auto totalDMG = _getTotal<Misc::SkillStat::DMG>(element, source, modifier.DMG);
		auto totalAdditiveDMG = _getTotal<Misc::SkillStat::additiveDMG>(element, source, modifier.additiveDMG) + Formula::AdditiveMultiplier{};
		auto totalMultiplicativeDMG = _getTotal<Misc::SkillStat::multiplicativeDMG>(element, source, modifier.multiplicativeDMG);
		auto totalElevation = _getTotal<Misc::SkillStat::elevation>(element, source, modifier.elevation);
		auto totalCritRate = Formula::Clamp({}, _getTotal<Misc::SkillStat::critRate>(element, source, modifier.critRate) + Modifiers::total().cr, 0.f, 1.f);
		auto totalCritDMG = _getTotal<Misc::SkillStat::critDMG>(element, source, modifier.critDMG) + Modifiers::total().cd;

		auto multiplier = (1.0f + totalMultiplicativeDMG) * formula + totalAdditiveDMG;
		auto elevation = 1.0f + totalElevation;
		auto dmgBonus = (1.0f + totalDMG);
		auto crit = 1.0f + totalCritRate * totalCritDMG;
		auto totalModifier = _getTotalEnemy(element, source, modifier.enemy);
		auto enemy = Formula::EnemyDefMultiplier{.modifiers = totalModifier} * Formula::EnemyResMultiplier{.attackSource = source, .element = element, .modifiers = totalModifier.resistance};
		auto amplifyingMultiplier = Formula::AmplifyingMultiplier{};

		return multiplier
			 * elevation
			 * dmgBonus
			 * crit
			 * enemy
			 * amplifyingMultiplier;
	}

}// namespace Node