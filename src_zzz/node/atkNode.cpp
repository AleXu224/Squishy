#include "atkNode.hpp"
#include "formula/attribute.hpp"
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
	struct _NodeAttribute : Formula::FormulaBase<float> {
		Utils::JankyOptional<Misc::Attribute> attribute{};
		Utils::JankyOptional<Misc::AttackSource> source{};
		Misc::SkillStat skillStat;

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			return Stats::fromSkillStat(Stats::fromAttribute(Modifiers::combat(), Formula::getAttribute(source, attribute, context)), skillStat).fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(
				std::format(
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

	struct _NodeSkill : Formula::FormulaBase<float> {
		Utils::JankyOptional<Misc::AttackSource> source{};
		Misc::SkillStat skillStat;

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			if (!source.has_value()) return Formula::Constant{.value = 0.f};
			return Stats::fromAttackSource(Modifiers::combat(), source.value(), skillStat).fold(context, args);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(
				std::format(
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

	[[nodiscard]] static constexpr auto _getTotal(
		Utils::JankyOptional<Misc::Attribute> attackAttribute,
		const Utils::JankyOptional<Misc::AttackSource> &atkSource,
		const Misc::SkillStat skillStat,
		const auto &formula
	) {
		auto allStats = Stats::fromSkillStat(Modifiers::combat().all, skillStat);
		auto attributeStats = _NodeAttribute({}, attackAttribute, atkSource, skillStat);
		auto skillStats = _NodeSkill({}, atkSource, skillStat);

		return allStats + attributeStats + skillStats + formula;
	}

	Formula::FloatNode Atk::_getFormula(
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

		auto multiplier = (1.0f + totalMultiplicativeDMG) * formula + totalAdditiveDMG;
		auto dmgBonus = (1.0f + totalDMG);
		auto crit = 1.0f + totalCritRate * totalCritDMG;
		auto enemy = Formula::EnemyDefMultiplier{} * Formula::EnemyResMultiplier({}, source, attribute);

		auto stunMod = Formula::Requires{.requirement = Requirement::enemyStunned, .ret = Modifiers::enemy().stunMod};
		// FIXME: dmg taken multiplier (piper)

		return multiplier
			 * dmgBonus
			 * crit
			 * enemy
			 * (1.f + stunMod);
	}

}// namespace Node