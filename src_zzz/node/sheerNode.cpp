#include "sheerNode.hpp"
#include "formula/clamp.hpp"
#include "formula/enemy.hpp"
#include "formula/nodes.hpp"
#include "formula/operators.hpp"
#include "formula/requirement.hpp"
#include "formula/requires.hpp"
#include "misc/attribute.hpp"
#include "misc/skillStat.hpp"
#include "modifiers/total/total.hpp"


namespace Node {
	using namespace Formula::Operators;
	[[nodiscard]] static constexpr auto _getTotal(
		Utils::JankyOptional<Misc::Attribute> attackAttribute,
		const Utils::JankyOptional<Misc::AttackSource> &atkSource,
		const Misc::SkillStat skillStat,
		const auto &formula
	) {
		auto allStats = Stats::fromSkillStat(Modifiers::combat().all, skillStat);
		auto sheerStats = Stats::fromSkillStat(Modifiers::combat().sheer, skillStat);
		auto attributeStats = Formula::NodeAttribute({}, attackAttribute, skillStat);
		auto skillStats = Formula::NodeSkill({}, atkSource, skillStat);

		return allStats + sheerStats + attributeStats + skillStats + formula;
	}

	[[nodiscard]] constexpr auto _getTotalEnemySheer(
		Utils::JankyOptional<Misc::Attribute> attackAttribute,
		const Utils::JankyOptional<Misc::AttackSource> &source,
		const auto &formula
	) {
		Formula::EnemyModifier modifiers{};
		modifiers = modifiers + Modifiers::combat().all.enemy
				  + Modifiers::combat().sheer.enemy
				  + Formula::getAttributeModifier(attackAttribute)
				  + Formula::getSourceModifier(source)
				  + formula;

		return modifiers;
	}

	Formula::FloatNode Sheer::_getFormula(
		const Utils::JankyOptional<Misc::Attribute> &attribute,
		const Utils::JankyOptional<Misc::AttackSource> &source,
		const Formula::FloatNode &formula,
		const Formula::Modifier &modifier
	) {
		auto totalDMG = _getTotal(attribute, source, Misc::SkillStat::DMG, modifier.DMG);
		auto totalAdditiveDMG = _getTotal(attribute, source, Misc::SkillStat::additiveDMG, modifier.additiveDMG);
		auto totalMultiplicativeDMG = _getTotal(attribute, source, Misc::SkillStat::multiplicativeDMG, modifier.multiplicativeDMG);
		auto totalCritRate = Formula::Clamp({}, _getTotal(attribute, source, Misc::SkillStat::critRate, modifier.critRate) + Modifiers::combat().cr, 0.f, 1.f);
		auto totalCritDMG = _getTotal(attribute, source, Misc::SkillStat::critDMG, modifier.critDMG) + Modifiers::combat().cd;

		auto multiplier = (1.0f + totalMultiplicativeDMG) * (formula * Modifiers::combat().sheerForce) + totalAdditiveDMG;
		auto dmgBonus = (1.0f + totalDMG);
		auto crit = 1.0f + totalCritRate * totalCritDMG;
		auto totalModifier = _getTotalEnemySheer(attribute, source, modifier.enemy);
		auto enemy = Formula::EnemyResMultiplier({}, attribute, totalModifier.resistance);

		auto stunMod = Formula::Requires{.requirement = Requirement::enemyStunned, .ret = Modifiers::enemy().stunMod};
		// FIXME: dmg taken multiplier (piper)

		return multiplier
			 * dmgBonus
			 * crit
			 * enemy
			 * (1.f + stunMod);
	}

}// namespace Node