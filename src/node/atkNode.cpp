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
		Misc::AttackSource source{};

		[[nodiscard]] auto compile(const Formula::Context &context) const {
			switch (Formula::getElement(source, element, context)) {
				case Misc::Element::pyro:
					return Stats::fromSkillStat(Modifiers::total().pyro, skillStat).compile(context);
				case Misc::Element::hydro:
					return Stats::fromSkillStat(Modifiers::total().hydro, skillStat).compile(context);
				case Misc::Element::cryo:
					return Stats::fromSkillStat(Modifiers::total().cryo, skillStat).compile(context);
				case Misc::Element::electro:
					return Stats::fromSkillStat(Modifiers::total().electro, skillStat).compile(context);
				case Misc::Element::dendro:
					return Stats::fromSkillStat(Modifiers::total().dendro, skillStat).compile(context);
				case Misc::Element::anemo:
					return Stats::fromSkillStat(Modifiers::total().anemo, skillStat).compile(context);
				case Misc::Element::geo:
					return Stats::fromSkillStat(Modifiers::total().geo, skillStat).compile(context);
				case Misc::Element::physical:
					return Stats::fromSkillStat(Modifiers::total().physical, skillStat).compile(context);
			}
			std::unreachable();
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(fmt::format("{} {}", Utils::Stringify(source), Utils::Stringify(skillStat)), eval(context), Utils::isPercentage(skillStat));
		}

		[[nodiscard]] static constexpr float switchElement(Misc::Element element, const Formula::Context &context) {
			switch (element) {
				case Misc::Element::pyro:
					return Stats::fromSkillStat(Modifiers::total().pyro, skillStat).eval(context);
				case Misc::Element::hydro:
					return Stats::fromSkillStat(Modifiers::total().hydro, skillStat).eval(context);
				case Misc::Element::cryo:
					return Stats::fromSkillStat(Modifiers::total().cryo, skillStat).eval(context);
				case Misc::Element::electro:
					return Stats::fromSkillStat(Modifiers::total().electro, skillStat).eval(context);
				case Misc::Element::dendro:
					return Stats::fromSkillStat(Modifiers::total().dendro, skillStat).eval(context);
				case Misc::Element::anemo:
					return Stats::fromSkillStat(Modifiers::total().anemo, skillStat).eval(context);
				case Misc::Element::geo:
					return Stats::fromSkillStat(Modifiers::total().geo, skillStat).eval(context);
				case Misc::Element::physical:
					return Stats::fromSkillStat(Modifiers::total().physical, skillStat).eval(context);
			}
			std::unreachable();
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return switchElement(Formula::getElement(source, element, context), context);
		}
	};

	template<Misc::SkillStat skillStat>
	struct _NodeSkill {
		Misc::AttackSource source{};

		[[nodiscard]] auto compile(const Formula::Context &context) const {
			switch (source) {
				case Misc::AttackSource::normal:
					return Stats::fromSkillStat(Modifiers::total().normal, skillStat).compile(context);
				case Misc::AttackSource::charged:
					return Stats::fromSkillStat(Modifiers::total().charged, skillStat).compile(context);
				case Misc::AttackSource::plunge:
					return Stats::fromSkillStat(Modifiers::total().plunge, skillStat).compile(context);
				case Misc::AttackSource::skill:
					return Stats::fromSkillStat(Modifiers::total().skill, skillStat).compile(context);
				case Misc::AttackSource::burst:
					return Stats::fromSkillStat(Modifiers::total().burst, skillStat).compile(context);
			}
			std::unreachable();
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(fmt::format("{} {}", Utils::Stringify(source), Utils::Stringify(skillStat)), eval(context), Utils::isPercentage(skillStat));
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			switch (source) {
				case Misc::AttackSource::normal:
					return Stats::fromSkillStat(Modifiers::total().normal, skillStat).eval(context);
				case Misc::AttackSource::charged:
					return Stats::fromSkillStat(Modifiers::total().charged, skillStat).eval(context);
				case Misc::AttackSource::plunge:
					return Stats::fromSkillStat(Modifiers::total().plunge, skillStat).eval(context);
				case Misc::AttackSource::skill:
					return Stats::fromSkillStat(Modifiers::total().skill, skillStat).eval(context);
				case Misc::AttackSource::burst:
					return Stats::fromSkillStat(Modifiers::total().burst, skillStat).eval(context);
			}
			std::unreachable();
		}
	};

	template<Misc::SkillStat skillStat>
	[[nodiscard]] constexpr auto _getTotal(
		Utils::JankyOptional<Misc::Element> attackElement,
		Misc::AttackSource atkSource,
		auto formula
	) {
		auto allStats = Stats::fromSkillStat(Modifiers::total().all, skillStat);
		auto elementStats = _NodeElement<skillStat>(attackElement, atkSource);
		auto skillStats = _NodeSkill<skillStat>(atkSource);

		return allStats + elementStats + skillStats + formula;
	}

	Formula::FloatNode Atk::_getFormula(
		const Utils::JankyOptional<Misc::Element> &element,
		Misc::AttackSource source,
		Formula::FloatNode formula,
		Formula::Modifier modifier
	) {
		auto totalDMG = _getTotal<Misc::SkillStat::DMG>(element, source, modifier.DMG);
		auto totalAdditiveDMG = _getTotal<Misc::SkillStat::additiveDMG>(element, source, modifier.additiveDMG) + Formula::AdditiveMultiplier{};
		auto totalMultiplicativeDMG = _getTotal<Misc::SkillStat::multiplicativeDMG>(element, source, modifier.multiplicativeDMG);
		auto totalCritRate = Formula::Clamp(_getTotal<Misc::SkillStat::critRate>(element, source, modifier.critRate) + Modifiers::total().cr, 0.f, 1.f);
		auto totalCritDMG = _getTotal<Misc::SkillStat::critDMG>(element, source, modifier.critDMG) + Modifiers::total().cd;

		auto multiplier = (1.0f + totalMultiplicativeDMG) * formula + totalAdditiveDMG;
		auto dmgBonus = (1.0f + totalDMG);
		auto crit = 1.0f + totalCritRate * totalCritDMG;
		auto enemy = Formula::EnemyDefMultiplier{} * Formula::EnemyResMultiplier(source, element);
		auto amplifyingMultiplier = Formula::AmplifyingMultiplier{};

		return multiplier
			 * dmgBonus
			 * crit
			 * enemy
			 * amplifyingMultiplier;
	}

}// namespace Node