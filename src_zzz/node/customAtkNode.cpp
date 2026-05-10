#include "customAtkNode.hpp"
#include "formula/clamp.hpp"
#include "formula/enemy.hpp"
#include "formula/operators.hpp"
#include "misc/attribute.hpp"
#include "misc/skillStat.hpp"
#include "modifiers/total/total.hpp"


namespace Node {
	using namespace Formula::Operators;
	template<Misc::SkillStat skillStat>
	struct _NodeAttribute : Formula::FormulaBase<float> {
		Misc::Attribute attribute;

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

	template<Misc::SkillStat skillStat>
	[[nodiscard]] static constexpr auto _getTotal(
		Misc::Attribute attackAttribute,
		const auto &formula
	) {
		auto allStats = Stats::fromSkillStat(Modifiers::combat().all, skillStat);
		auto attributeStats = _NodeAttribute<skillStat>({}, attackAttribute);

		return allStats + attributeStats + formula;
	}

	Formula::FloatNode CustomAtk::_getFormula(
		const Misc::Attribute &attribute,
		const Formula::FloatNode &formula,
		const Formula::Modifier &modifier
	) {
		auto totalDMG = _getTotal<Misc::SkillStat::DMG>(attribute, modifier.DMG);
		auto totalAdditiveDMG = _getTotal<Misc::SkillStat::additiveDMG>(attribute, modifier.additiveDMG);
		auto totalMultiplicativeDMG = _getTotal<Misc::SkillStat::multiplicativeDMG>(attribute, modifier.multiplicativeDMG);
		auto totalCritRate = Formula::Clamp({}, _getTotal<Misc::SkillStat::critRate>(attribute, modifier.critRate) + Modifiers::combat().cr, 0.f, 1.f);
		auto totalCritDMG = _getTotal<Misc::SkillStat::critDMG>(attribute, modifier.critDMG) + Modifiers::combat().cd;

		auto multiplier = (1.0f + totalMultiplicativeDMG) * formula + totalAdditiveDMG;
		auto dmgBonus = (1.0f + totalDMG);
		auto crit = 1.0f + totalCritRate * totalCritDMG;
		auto enemy = Formula::EnemyDefMultiplier{} * Formula::EnemyResMultiplier({}, Misc::AttackSource::basic, attribute);

		// FIXME: stun multiplier, dmg taken multiplier (piper)

		return multiplier
			 * dmgBonus
			 * crit
			 * enemy;
	}

}// namespace Node