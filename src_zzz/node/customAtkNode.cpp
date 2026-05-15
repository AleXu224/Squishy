#include "customAtkNode.hpp"
#include "formula/clamp.hpp"
#include "formula/enemy.hpp"
#include "formula/operators.hpp"
#include "formula/requirement.hpp"
#include "formula/requires.hpp"
#include "misc/attribute.hpp"
#include "misc/skillStat.hpp"
#include "modifiers/total/total.hpp"


namespace Node {
	using namespace Formula::Operators;
	struct _NodeAttributeAbloom : Formula::FormulaBase<float> {
		Misc::Attribute attribute;
		Misc::SkillStat skillStat;

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return Stats::fromSkillStat(Stats::fromAttribute(Modifiers::combat(), attribute), skillStat).fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(
				fmt::format(
					"{} {}",
					Utils::Stringify(attribute),
					Utils::Stringify(skillStat)
				),
				eval(context), Utils::isPercentage(skillStat)
			);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return Stats::fromSkillStat(Stats::fromAttribute(Modifiers::combat(), attribute), skillStat).eval(context);
		}
	};

	[[nodiscard]] static constexpr auto _getTotalCustom(
		Misc::Attribute attackAttribute,
		Misc::SkillStat skillStat,
		const auto &formula
	) {
		auto allStats = Stats::fromSkillStat(Modifiers::combat().all, skillStat);
		auto attributeStats = _NodeAttributeAbloom({}, attackAttribute, skillStat);

		return allStats + attributeStats + formula;
	}

	Formula::FloatNode CustomAtk::_getFormula(
		const Misc::Attribute &attribute,
		const Formula::FloatNode &formula,
		const Formula::Modifier &modifier
	) {
		auto totalDMG = _getTotalCustom(attribute, Misc::SkillStat::DMG, modifier.DMG);
		auto totalAdditiveDMG = _getTotalCustom(attribute, Misc::SkillStat::additiveDMG, modifier.additiveDMG);
		auto totalMultiplicativeDMG = _getTotalCustom(attribute, Misc::SkillStat::multiplicativeDMG, modifier.multiplicativeDMG);
		auto totalCritRate = Formula::Clamp({}, _getTotalCustom(attribute, Misc::SkillStat::critRate, modifier.critRate) + Modifiers::combat().cr, 0.f, 1.f);
		auto totalCritDMG = _getTotalCustom(attribute, Misc::SkillStat::critDMG, modifier.critDMG) + Modifiers::combat().cd;

		auto multiplier = (1.0f + totalMultiplicativeDMG) * formula + totalAdditiveDMG;
		auto dmgBonus = (1.0f + totalDMG);
		auto crit = 1.0f + totalCritRate * totalCritDMG;
		auto enemy = Formula::EnemyDefMultiplier{} * Formula::EnemyResMultiplier({}, Misc::AttackSource::basic, attribute);

		auto stunMod = Formula::Requires{.requirement = Requirement::enemyStunned, .ret = Modifiers::enemy().stunMod};
		// FIXME: dmg taken multiplier (piper)

		return multiplier
			 * dmgBonus
			 * crit
			 * enemy
			 * (1.f + stunMod);
	}

}// namespace Node