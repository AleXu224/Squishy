#include "customAtkNode.hpp"
#include "formula/nodes.hpp"
#include "formula/operators.hpp"
#include "formula/reaction.hpp"
#include "misc/element.hpp"
#include "misc/skillStat.hpp"
#include "modifiers/total/total.hpp"


namespace Node {
	using namespace Formula::Operators;

	template<Misc::SkillStat skillStat>
	[[nodiscard]] constexpr auto _getCustomAtkTotal(
		Misc::Element attackElement,
		auto formula
	) {
		auto allStats = Stats::fromSkillStat(Modifiers::total().all, skillStat);
		auto elementStats = Formula::NodeElementSimple({}, attackElement, skillStat);

		return allStats + elementStats + formula;
	}

	[[nodiscard]] constexpr auto _getTotalEnemy(
		Misc::Element attackElement,
		const auto &formula
	) {
		Formula::EnemyModifier modifiers{};
		modifiers = modifiers + Modifiers::total().all.enemy
				  + Formula::getElementModifierSimple(attackElement)
				  + formula;

		return modifiers;
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
		auto totalModifier = _getTotalEnemy(element, modifier.enemy);
		auto enemy = Formula::EnemyDefMultiplier{.modifiers = totalModifier}
				   * Formula::EnemyResMultiplier{.attackSource = {}, .element = element, .modifiers = totalModifier.resistance};
		auto amplifyingMultiplier = Formula::AmplifyingMultiplier{};

		return multiplier
			 * elevation
			 * dmgBonus
			 * enemy
			 * amplifyingMultiplier;
	}

}// namespace Node