#include "abloomNode.hpp"
#include "formula/anomaly.hpp"
#include "formula/clamp.hpp"
#include "formula/nodes.hpp"
#include "formula/operators.hpp"
#include "formula/teamAgent.hpp"
#include "misc/attribute.hpp"
#include "misc/skillStat.hpp"
#include "modifiers/total/total.hpp"


namespace Node {
	using namespace Formula::Operators;
	[[nodiscard]] static constexpr auto _getTotalAbloom(
		Misc::SkillStat skillStat,
		const auto &formula
	) {
		auto allAnomaly = Stats::fromSkillStat(Modifiers::combat().allAnomaly, skillStat);
		auto abloom = Stats::fromSkillStat(Modifiers::combat().abloom, skillStat);

		return allAnomaly + abloom + formula;
	}

	[[nodiscard]] constexpr auto _getTotalEnemyCustom(
		Formula::NodeType<Misc::Attribute> attackAttribute,
		const auto &formula
	) {
		Formula::EnemyModifier modifiers{};
		modifiers = modifiers + Modifiers::combat().allAnomaly.enemy
				  + Modifiers::combat().abloom.enemy
				  + Formula::getAttributeModifierDynamic(attackAttribute)
				  + formula;

		return modifiers;
	}

	Formula::FloatNode AbloomNode::_getFormulaAbloom(
		const Formula::NodeType<Misc::Attribute> &attribute,
		const Formula::FloatNode &multiplier,
		size_t index,
		const Formula::Modifier &modifier
	) {
		auto totalDMG = _getTotalAbloom(Misc::SkillStat::DMG, modifier.DMG);
		auto totalAdditiveDMG = _getTotalAbloom(Misc::SkillStat::additiveDMG, modifier.additiveDMG);
		// auto totalMultiplicativeDMG = _getTotalCustom(Misc::SkillStat::multiplicativeDMG, modifier.multiplicativeDMG);
		auto totalCritRate = Formula::Clamp({}, _getTotalAbloom(Misc::SkillStat::critRate, modifier.critRate), 0.f, 1.f);
		auto totalCritDMG = _getTotalAbloom(Misc::SkillStat::critDMG, modifier.critDMG);
		auto totalModifier = _getTotalEnemyCustom(attribute, modifier.enemy);
		auto resMod = Formula::EnemyResMultiplierDynamic{.element = attribute, .modifiers = totalModifier.resistance};

		auto formula = Formula::TeamAgent{
						   .index = index,
						   .formula = Formula::AnomalyAbloomFromAttribute{
							   .attribute = attribute,
						   },
					   } * (1.f + totalDMG)
					 + totalAdditiveDMG;
		auto crit = 1.0f + totalCritRate * totalCritDMG;

		return multiplier
			 * formula
			 * resMod
			 * crit;
	}

}// namespace Node