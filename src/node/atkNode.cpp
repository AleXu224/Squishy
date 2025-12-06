#include "atkNode.hpp"
#include "formula/element.hpp"
#include "formula/operators.hpp"
#include "formula/reaction.hpp"
#include "misc/element.hpp"
#include "misc/skillStat.hpp"
#include "modifiers/total/total.hpp"


namespace Node {
	using namespace Formula::Operators;
	template<Misc::SkillStat skillStat>
	struct _NodeElement {
		Utils::JankyOptional<Misc::Element> element{};
		Utils::JankyOptional<Misc::AttackSource> source{};

		[[nodiscard]] auto compile(const Formula::Context &context) const {
			return Stats::fromSkillStat(Stats::fromElement(Modifiers::total(), Formula::getElement(source, element, context)), skillStat).compile(context);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(
				fmt::format(
					"{} {}",
					Utils::Stringify(Formula::getElement(source, element, context)),
					Utils::Stringify(skillStat)
				),
				eval(context), Utils::isPercentage(skillStat)
			);
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return Stats::fromSkillStat(Stats::fromElement(Modifiers::total(), Formula::getElement(source, element, context)), skillStat).eval(context);
		}
	};

	template<Misc::SkillStat skillStat>
	struct _NodeSkill {
		Utils::JankyOptional<Misc::AttackSource> source{};

		[[nodiscard]] Formula::Compiled::FloatNode compile(const Formula::Context &context) const {
			if (!source.has_value()) return Formula::Compiled::ConstantFloat{.value = 0.f};
			return Stats::fromAttackSource(Modifiers::total(), source.value(), skillStat).compile(context);
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
			return Stats::fromAttackSource(Modifiers::total(), source.value(), skillStat).eval(context);
		}
	};

	template<Misc::SkillStat skillStat>
	[[nodiscard]] static constexpr auto _getTotal(
		Utils::JankyOptional<Misc::Element> attackElement,
		const Utils::JankyOptional<Misc::AttackSource> &atkSource,
		const auto &formula
	) {
		auto allStats = Stats::fromSkillStat(Modifiers::total().all, skillStat);
		auto elementStats = _NodeElement<skillStat>(attackElement, atkSource);
		auto skillStats = _NodeSkill<skillStat>(atkSource);

		return allStats + elementStats + skillStats + formula;
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
		auto totalCritRate = Formula::Clamp(_getTotal<Misc::SkillStat::critRate>(element, source, modifier.critRate) + Modifiers::total().cr, 0.f, 1.f);
		auto totalCritDMG = _getTotal<Misc::SkillStat::critDMG>(element, source, modifier.critDMG) + Modifiers::total().cd;

		auto multiplier = (1.0f + totalMultiplicativeDMG) * formula + totalAdditiveDMG;
		auto elevation = 1.0f + totalElevation;
		auto dmgBonus = (1.0f + totalDMG);
		auto crit = 1.0f + totalCritRate * totalCritDMG;
		auto enemy = Formula::EnemyDefMultiplier{} * Formula::EnemyResMultiplier(source, element);
		auto amplifyingMultiplier = Formula::AmplifyingMultiplier{};

		return multiplier
			 * elevation
			 * dmgBonus
			 * crit
			 * enemy
			 * amplifyingMultiplier;
	}

}// namespace Node