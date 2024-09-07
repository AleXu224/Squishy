#pragma once

#include "formula/attackModifier.hpp"
#include "formula/clamp.hpp"
#include "formula/enemy.hpp"
#include "formula/operators.hpp"
#include "formula/reaction.hpp"
#include "formula/stat.hpp"
#include "misc/attackSource.hpp"
#include "misc/element.hpp"
#include "nodeData.hpp"
#include "stats/sheet.hpp"
#include "string_view"


namespace Node {
	using _Sheet = decltype(Stats::CharacterSheet::postMods);
	using _Stats = Stats::CharacterSheet;

	template<class T>
	[[nodiscard]] constexpr auto _getTotal(
		Utils::JankyOptional<Misc::Element> attackElement,
		Misc::AttackSource atkSource,
		_Sheet::_SkillValue _Sheet::*skill,
		_Sheet::_Value _Sheet::_SkillValue::*stat,
		T formula
	) {
		auto allStats = Formula::SkillPtr(&_Stats::postMods, &_Sheet::all, stat);
		auto elementStats = Formula::ElementStat(atkSource, attackElement, stat);
		auto skillStats = Formula::SkillPtr(&_Stats::postMods, skill, stat);

		return allStats + elementStats + skillStats + formula;
	}

	template<class Frm, class T = decltype(Formula::Modifier{})>
	struct Atk {
		std::string_view name;
		Utils::JankyOptional<Misc::Element> element{};
		Misc::AttackSource source{};
		Frm formula;
		T modifier{};

		[[nodiscard]] static constexpr auto _getFormula(
			const Utils::JankyOptional<Misc::Element> &element,
			Misc::AttackSource source,
			Frm formula,
			T modifier
		) {
			auto skill = Stats::getSheetMemberByAttackSource<_Sheet>(source);

			auto totalDMG = _getTotal(element, source, skill, &_Sheet::_SkillValue::DMG, modifier.DMG);
			auto totalAdditiveDMG = _getTotal(element, source, skill, &_Sheet::_SkillValue::additiveDMG, modifier.additiveDMG) + Formula::AdditiveMultiplier{};
			auto totalMultiplicativeDMG = _getTotal(element, source, skill, &_Sheet::_SkillValue::multiplicativeDMG, modifier.multiplicativeDMG);
			auto totalCritRate = Formula::Clamp(_getTotal(element, source, skill, &_Sheet::_SkillValue::critRate, modifier.critRate) + Formula::CharacterStat(Stat::cr), 0.f, 1.f);
			auto totalCritDMG = _getTotal(element, source, skill, &_Sheet::_SkillValue::critDMG, modifier.critDMG) + Formula::CharacterStat(Stat::cd);

			auto multiplier = (1.0f + totalMultiplicativeDMG) * formula + totalAdditiveDMG;
			auto dmgBonus = (1.0f + totalDMG);
			auto crit = 1.0f + totalCritRate * totalCritDMG;
			auto enemy = Formula::EnemyDefMultiplier{} * Formula::EnemyResMultiplier(source, element);
			auto amplifyingMultiplier = Formula::AmplifyingMultiplier{};

			return multiplier * dmgBonus * crit * enemy * amplifyingMultiplier;
		}

		using _FormulaRetType = decltype(_getFormula(
			std::declval<const Utils::JankyOptional<Misc::Element> &>(),
			std::declval<Misc::AttackSource>(),
			std::declval<Frm>(),
			std::declval<T>()
		));

		_FormulaRetType _formula = _getFormula(element, source, formula, modifier);

		Data _data = AtkData{
			.element = element,
			.source = source,
		};
	};
}// namespace Node