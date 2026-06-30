#include "anomaly.hpp"

#include "formula/anomaly.hpp"
#include "formula/nodes.hpp"
#include "formula/teamAgent.hpp"

namespace Anomaly {
	[[nodiscard]] static constexpr auto _getTotalVortex(
		Misc::SkillStat skillStat,
		const auto &formula
	) {
		auto allAnomaly = Stats::fromSkillStat(Modifiers::combat().allAnomaly, skillStat);

		return allAnomaly + formula;
	}

	[[nodiscard]] constexpr auto _getTotalEnemyCustom(
		Formula::NodeType<Misc::Attribute> attackAttribute,
		const auto &formula
	) {
		Formula::EnemyModifier modifiers{};
		modifiers = modifiers + Modifiers::combat().allAnomaly.enemy
				  + Formula::getAttributeModifierDynamic(attackAttribute)
				  + formula;

		return modifiers;
	}

	Formula::FloatNode makeVortexFormula(const Stats::Sheet<Formula::FloatNode>::_SkillValue &modifier, Formula::NodeType<Misc::Attribute> attribute, size_t index) {
		auto totalDMG = _getTotalVortex(Misc::SkillStat::DMG, modifier.DMG);
		auto totalAdditiveDMG = _getTotalVortex(Misc::SkillStat::additiveDMG, modifier.additiveDMG);
		// auto totalMultiplicativeDMG = _getTotalCustom(Misc::SkillStat::multiplicativeDMG, modifier.multiplicativeDMG);
		auto totalCritRate = Formula::Clamp({}, _getTotalVortex(Misc::SkillStat::critRate, modifier.critRate), 0.f, 1.f);
		auto totalCritDMG = _getTotalVortex(Misc::SkillStat::critDMG, modifier.critDMG);
		auto totalModifier = _getTotalEnemyCustom(attribute, modifier.enemy);
		auto resMod = Formula::EnemyResMultiplierDynamic{.element = attribute, .modifiers = totalModifier.resistance};

		auto formula = Formula::TeamAgent{
						   .index = index,
						   .formula = Formula::AnomalyVortexFromAttribute{
							   .attribute = attribute,
						   },
					   } * (1.f + totalDMG)
					 + totalAdditiveDMG;
		auto crit = 1.0f + totalCritRate * totalCritDMG;

		return formula
			 * resMod
			 * crit;
	}
}// namespace Anomaly