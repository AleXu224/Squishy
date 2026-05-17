#include "customAtkNode.hpp"
#include "formula/operators.hpp"
#include "formula/reaction.hpp"
#include "misc/element.hpp"
#include "misc/skillStat.hpp"
#include "modifiers/total/total.hpp"


namespace Node {
	using namespace Formula::Operators;
	template<Misc::SkillStat skillStat>
	struct _CustomAtkNodeElement : Formula::FormulaBase<float> {
		Misc::Element element;

		[[nodiscard]] Formula::FloatNode fold(const Formula::Context &context, const Formula::FoldArgs &args) const {
			switch (element) {
				case Misc::Element::pyro:
					return Stats::fromSkillStat(Modifiers::total().pyro, skillStat).fold(context, args);
				case Misc::Element::hydro:
					return Stats::fromSkillStat(Modifiers::total().hydro, skillStat).fold(context, args);
				case Misc::Element::cryo:
					return Stats::fromSkillStat(Modifiers::total().cryo, skillStat).fold(context, args);
				case Misc::Element::electro:
					return Stats::fromSkillStat(Modifiers::total().electro, skillStat).fold(context, args);
				case Misc::Element::dendro:
					return Stats::fromSkillStat(Modifiers::total().dendro, skillStat).fold(context, args);
				case Misc::Element::anemo:
					return Stats::fromSkillStat(Modifiers::total().anemo, skillStat).fold(context, args);
				case Misc::Element::geo:
					return Stats::fromSkillStat(Modifiers::total().geo, skillStat).fold(context, args);
				case Misc::Element::physical:
					return Stats::fromSkillStat(Modifiers::total().physical, skillStat).fold(context, args);
			}
			std::unreachable();
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(std::format("{}", Utils::Stringify(skillStat)), eval(context), Utils::isPercentage(skillStat));
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
			return switchElement(element, context);
		}
	};

	template<Misc::SkillStat skillStat>
	[[nodiscard]] constexpr auto _getCustomAtkTotal(
		Misc::Element attackElement,
		auto formula
	) {
		auto allStats = Stats::fromSkillStat(Modifiers::total().all, skillStat);
		auto elementStats = _CustomAtkNodeElement<skillStat>({}, attackElement);

		return allStats + elementStats + formula;
	}

	Formula::FloatNode CustomAtk::_getFormula(
		Misc::Element element,
		Formula::FloatNode formula,
		Formula::CustomAtkModifier modifier
	) {
		auto totalDMG = _getCustomAtkTotal<Misc::SkillStat::DMG>(element, modifier.DMG);
		auto totalAdditiveDMG = _getCustomAtkTotal<Misc::SkillStat::additiveDMG>(element, modifier.additiveDMG) + Formula::AdditiveMultiplier{};
		auto totalMultiplicativeDMG = _getCustomAtkTotal<Misc::SkillStat::multiplicativeDMG>(element, modifier.multiplicativeDMG);
		auto totalElevation = _getCustomAtkTotal<Misc::SkillStat::elevation>(element, modifier.elevation);

		auto multiplier = (1.0f + totalMultiplicativeDMG) * formula + totalAdditiveDMG;
		auto elevation = 1.0f + totalElevation;
		auto dmgBonus = (1.0f + totalDMG);
		auto enemy = Formula::EnemyDefMultiplier{} * Formula::EnemyResMultiplier{.attackSource = {}, .element = element};
		auto amplifyingMultiplier = Formula::AmplifyingMultiplier{};

		return multiplier
			 * elevation
			 * dmgBonus
			 * enemy
			 * amplifyingMultiplier;
	}

}// namespace Node