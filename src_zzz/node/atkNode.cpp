#include "atkNode.hpp"
#include "formula/attribute.hpp"
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
		Utils::JankyOptional<Misc::Attribute> attribute{};
		Utils::JankyOptional<Misc::AttackSource> source{};

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return Stats::fromSkillStat(Stats::fromAttribute(Modifiers::combat(), Formula::getAttribute(source, attribute, context)), skillStat).fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(
				fmt::format(
					"{} {}",
					Utils::Stringify(Formula::getAttribute(source, attribute, context)),
					Utils::Stringify(skillStat)
				),
				eval(context), Utils::isPercentage(skillStat)
			);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return Stats::fromSkillStat(Stats::fromAttribute(Modifiers::combat(), Formula::getAttribute(source, attribute, context)), skillStat).eval(context);
		}
	};

	template<Misc::SkillStat skillStat>
	struct _NodeSkill : Formula::FormulaBase<float> {
		Utils::JankyOptional<Misc::AttackSource> source{};

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			if (!source.has_value()) return Formula::Constant{.value = 0.f};
			return Stats::fromAttackSource(Modifiers::combat(), source.value(), skillStat).fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(
				fmt::format(
					"{} {}",
					Utils::Stringify(source),
					Utils::Stringify(skillStat)
				),
				eval(context), Utils::isPercentage(skillStat)
			);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			if (!source.has_value()) return 0.f;
			return Stats::fromAttackSource(Modifiers::combat(), source.value(), skillStat).eval(context);
		}
	};

	template<Misc::SkillStat skillStat>
	[[nodiscard]] static constexpr auto _getTotal(
		Utils::JankyOptional<Misc::Attribute> attackAttribute,
		const Utils::JankyOptional<Misc::AttackSource> &atkSource,
		const auto &formula
	) {
		auto allStats = Stats::fromSkillStat(Modifiers::combat().all, skillStat);
		auto attributeStats = _NodeAttribute<skillStat>({}, attackAttribute, atkSource);
		auto skillStats = _NodeSkill<skillStat>({}, atkSource);

		return allStats + attributeStats + skillStats + formula;
	}

	Formula::FloatNode Atk::_getFormula(
		const Utils::JankyOptional<Misc::Attribute> &attribute,
		const Utils::JankyOptional<Misc::AttackSource> &source,
		const Formula::FloatNode &formula,
		const Formula::Modifier &modifier
	) {
		auto totalDMG = _getTotal<Misc::SkillStat::DMG>(attribute, source, modifier.DMG);
		auto totalAdditiveDMG = _getTotal<Misc::SkillStat::additiveDMG>(attribute, source, modifier.additiveDMG);
		auto totalMultiplicativeDMG = _getTotal<Misc::SkillStat::multiplicativeDMG>(attribute, source, modifier.multiplicativeDMG);
		auto totalCritRate = Formula::Clamp({}, _getTotal<Misc::SkillStat::critRate>(attribute, source, modifier.critRate) + Modifiers::combat().cr, 0.f, 1.f);
		auto totalCritDMG = _getTotal<Misc::SkillStat::critDMG>(attribute, source, modifier.critDMG) + Modifiers::combat().cd;

		auto multiplier = (1.0f + totalMultiplicativeDMG) * formula + totalAdditiveDMG;
		auto dmgBonus = (1.0f + totalDMG);
		auto crit = 1.0f + totalCritRate * totalCritDMG;
		auto enemy = Formula::EnemyDefMultiplier{} * Formula::EnemyResMultiplier({}, source, attribute);

		// FIXME: stun multiplier, dmg taken multiplier (piper)

		return multiplier
			 * dmgBonus
			 * crit
			 * enemy;
	}

}// namespace Node