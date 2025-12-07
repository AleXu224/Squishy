#include "directLunar.hpp"

#include "formula/clamp.hpp"
#include "formula/enemy.hpp"
#include "formula/operators.hpp"
#include "modifiers/total/total.hpp"
#include <utility>


namespace Node {
	using namespace Formula::Operators;

	template<Misc::SkillStat skillStat>
	struct _DirectLunarNodeElement {
		Misc::LunarDamageType damageType{};

		[[nodiscard]] constexpr const Formula::FloatNode &switchLunarDamageType() const {
			switch (damageType) {
				case Misc::LunarDamageType::lunarCharged:
					return Stats::fromSkillStat(Modifiers::total().lunarCharged, skillStat);
				case Misc::LunarDamageType::lunarBloom:
					return Stats::fromSkillStat(Modifiers::total().lunarBloom, skillStat);
				case Misc::LunarDamageType::lunarCrystallize:
					return Stats::fromSkillStat(Modifiers::total().lunarCrystallize, skillStat);
			}
			std::unreachable();
		}

		[[nodiscard]] auto compile(const Formula::Context &context) const {
			return switchLunarDamageType().compile(context);
		}

		[[nodiscard]] std::string print(const Formula::Context &context, Formula::Step) const {
			return Formula::Percentage(fmt::format("{} {}", Utils::Stringify(damageType), Utils::Stringify(skillStat)), eval(context), Utils::isPercentage(skillStat));
		}

		[[nodiscard]] float eval(const Formula::Context &context) const {
			return switchLunarDamageType().eval(context);
		}
	};

	template<Misc::SkillStat skillStat>
	[[nodiscard]] static constexpr auto _getTotal(
		Misc::LunarDamageType damageType,
		const auto &formula
	) {
		auto elementStats = _DirectLunarNodeElement<skillStat>(damageType);

		return elementStats + formula;
	}

	[[nodiscard]] static float getDirectLunarMultiplier(Misc::LunarDamageType damageType) {
		switch (damageType) {
			case Misc::LunarDamageType::lunarCharged:
				return 3.f;
			case Misc::LunarDamageType::lunarBloom:
				return 1.f;
			case Misc::LunarDamageType::lunarCrystallize:
				return 1.6f;
		}
		std::unreachable();
	}

	Formula::FloatNode DirectLunar::_getFormula(
		Misc::LunarDamageType damageType,
		const Formula::FloatNode &formula,
		const Formula::Modifier &modifier
	) {
		auto totalDMG = modifier.DMG + _DirectLunarNodeElement<Misc::SkillStat::DMG>(damageType) + Modifiers::total().allLunar.DMG;
		auto totalMultiplicativeDMG = modifier.multiplicativeDMG + _DirectLunarNodeElement<Misc::SkillStat::multiplicativeDMG>(damageType) + Modifiers::total().allLunar.multiplicativeDMG;
		auto totalAdditiveDMG = modifier.additiveDMG + _DirectLunarNodeElement<Misc::SkillStat::additiveDMG>(damageType) + Modifiers::total().allLunar.additiveDMG;
		auto totalElevation = modifier.elevation + _DirectLunarNodeElement<Misc::SkillStat::elevation>(damageType) + Modifiers::total().allLunar.elevation;
		auto totalCritRate = Formula::Clamp(modifier.critRate + _DirectLunarNodeElement<Misc::SkillStat::critRate>(damageType) + Modifiers::total().allLunar.critRate + Modifiers::total().cr + Stats::fromElement(Modifiers::total(), Misc::lunarDamageTypeToElement(damageType)).critRate, 0.f, 1.f);
		auto totalCritDMG = modifier.critDMG + _DirectLunarNodeElement<Misc::SkillStat::critDMG>(damageType) + Modifiers::total().allLunar.critDMG + Modifiers::total().cd + Stats::fromElement(Modifiers::total(), Misc::lunarDamageTypeToElement(damageType)).critDMG;

		auto multiplier = (1.0f + totalMultiplicativeDMG) * formula;
		auto emBonus = (6.f * Modifiers::total().em) / (Modifiers::total().em + Formula::ConstantFlat(2000.f));
		auto dmgBonus = (1.0f + emBonus + totalDMG);
		auto crit = 1.0f + totalCritRate * totalCritDMG;
		auto enemy = Formula::EnemyResMultiplier(Misc::AttackSource::burst, Misc::lunarDamageTypeToElement(damageType));
		auto elevation = 1.0f + totalElevation;

		auto finalMultiplier = getDirectLunarMultiplier(damageType);

		return (multiplier * dmgBonus * finalMultiplier
				+ totalAdditiveDMG)
			 * crit
			 * enemy
			 * elevation;
	}

}// namespace Node