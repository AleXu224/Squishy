#pragma once

#include "formula/attackModifier.hpp"
#include "formula/clamp.hpp"
#include "formula/enemy.hpp"
#include "formula/operators.hpp"
#include "formula/reaction.hpp"
#include "formula/stat.hpp"
#include "misc/attackSource.hpp"
#include "misc/element.hpp"
#include "stats/sheet.hpp"
#include "string_view"


namespace Node {
	using _Sheet = decltype(Stats::CharacterSheet::postMods);
	using _Stats = Stats::CharacterSheet;

	template<class T>
	struct _NodeRet {
		std::string_view name;
		Utils::JankyOptional<Misc::Element> element;
		Misc::AttackSource source = Misc::AttackSource::normal;
		T formula;
	};

	template<class T>
	[[nodiscard]] consteval auto _getTotal(
		Utils::JankyOptional<Misc::Element> attackElement,
		Misc::AttackSource atkSource,
		_Sheet::_SkillValue _Sheet::*skill,
		_Sheet::_Value _Sheet::_SkillValue::*stat,
		T formula
	) {
		auto allStats = Formula::SkillPtr(&_Stats::postMods, &_Sheet::all, stat);
		auto elementStats = Formula::ElementStat(atkSource, attackElement, stat);
		auto skillStats = Formula::SkillPtr(&_Stats::postMods, skill, stat);

		return allStats +
			   elementStats +
			   skillStats +
			   formula;
	}

	template<Formula::FloatFormula Frm, class Tpl>
	[[nodiscard]] consteval auto _getFormula(std::string_view name, Utils::JankyOptional<Misc::Element> element, Misc::AttackSource source, Frm formula, Tpl stats) {
		auto skill = Stats::getSheetMemberByAttackSource<_Sheet>(source);

		auto totalDMG = _getTotal(element, source, skill, &_Sheet::_SkillValue::DMG, stats.DMG);
		auto totalAdditiveDMG = _getTotal(element, source, skill, &_Sheet::_SkillValue::additiveDMG, stats.additiveDMG) + Formula::AdditiveMultiplier{};
		auto totalMultiplicativeDMG = _getTotal(element, source, skill, &_Sheet::_SkillValue::multiplicativeDMG, stats.multiplicativeDMG);
		auto totalCritRate = Formula::Clamp(_getTotal(element, source, skill, &_Sheet::_SkillValue::critRate, stats.critRate) + Formula::CharacterStat(Stat::cr), 0.f, 1.f);
		auto totalCritDMG = _getTotal(element, source, skill, &_Sheet::_SkillValue::critDMG, stats.critDMG) + Formula::CharacterStat(Stat::cd);

		auto multiplier = (1.0f + totalMultiplicativeDMG) * formula + totalAdditiveDMG;
		auto dmgBonus = (1.0f + totalDMG);
		auto crit = 1.0f + totalCritRate * totalCritDMG;
		auto enemy = Formula::EnemyDefMultiplier{} * Formula::EnemyResMultiplier(source, element);
		auto amplifyingMultiplier = Formula::AmplifyingMultiplier{};

		return _NodeRet(
			name,
			element,
			source,
			multiplier * dmgBonus * crit * enemy * amplifyingMultiplier
		);
	}

	template<Formula::FloatFormula Frm>
	[[nodiscard]] consteval auto Atk(std::string_view name, Misc::AttackSource source, Frm formula) {
		return _getFormula(name, {}, source, formula, Formula::Modifier{});
	}

	template<Formula::FloatFormula Frm>
	[[nodiscard]] consteval auto Atk(std::string_view name, Misc::Element element, Misc::AttackSource source, Frm formula) {
		return _getFormula(name, element, source, formula, Formula::Modifier{});
	}

	template<Formula::FloatFormula Frm, class T>
	[[nodiscard]] consteval auto Atk(std::string_view name, Misc::AttackSource source, Frm formula, T modifiers) {
		return _getFormula(name, {}, source, formula, modifiers);
	}
	template<Formula::FloatFormula Frm, class T>
	[[nodiscard]] consteval auto Atk(std::string_view name, Misc::Element element, Misc::AttackSource source, Frm formula, T modifiers) {
		return _getFormula(name, element, source, formula, modifiers);
	}
}// namespace Node