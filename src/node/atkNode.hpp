#pragma once

#include "formula/clamp.hpp"
#include "formula/operators.hpp"
#include "formula/stat.hpp"
#include "misc/attackSource.hpp"
#include "misc/element.hpp"
#include "stats/character.hpp"
#include "stats/sheet.hpp"
#include "string_view"


namespace Node {
	using _Sheet = decltype(decltype(Stats::CharacterSheet::stats)::postMods);
	using _Stats = decltype(Stats::CharacterSheet::stats);

	template<class T>
	struct _NodeRet {
		std::string_view name;
		Utils::JankyOptional<Misc::Element> element;
		Misc::AttackSource source;
		T formula;
	};

	template<class T>
	[[nodiscard]] consteval auto _getTotal(
		Utils::JankyOptional<Misc::Element> attackElement,
		Misc::AttackSource atkSource,
		_Sheet::_SkillValue _Sheet:: *skill,
		_Sheet::_Value _Sheet::_SkillValue:: *stat,
		T formula
	) {
		auto allStats = Formula::SkillPtr<_Sheet, Stats::CharacterSheet>(&_Sheet::all, stat, &_Stats::postMods);
		auto elementStats = Formula::ElementStat<_Sheet>(atkSource, attackElement, stat);
		auto skillStats = Formula::SkillPtr<_Sheet, Stats::CharacterSheet>(skill, stat, &_Stats::postMods);

		return allStats +
			   elementStats +
			   skillStats +
			   formula;
	}

	template<Formula::IntermediaryLike Frm, class Tpl>
	[[nodiscard]] consteval auto _getFormula(std::string_view name, Utils::JankyOptional<Misc::Element> element, Misc::AttackSource source, Frm formula, Tpl stats) {
		auto skill = Stats::getSheetMemberByAttackSource<_Sheet>(source);

		auto totalDMG = _getTotal(element, source, skill, &_Sheet::_SkillValue::DMG, std::get<0>(stats));
		auto totalAdditiveDMG = _getTotal(element, source, skill, &_Sheet::_SkillValue::additiveDMG, std::get<1>(stats));
		auto totalMultiplicativeDMG = _getTotal(element, source, skill, &_Sheet::_SkillValue::multiplicativeDMG, std::get<2>(stats));
		auto totalCritRate = Formula::Clamp(_getTotal(element, source, skill, &_Sheet::_SkillValue::critRate, std::get<3>(stats)) + Formula::Stat(Stat::cr), 0.f, 1.f);
		auto totalCritDMG = _getTotal(element, source, skill, &_Sheet::_SkillValue::critDMG, std::get<4>(stats)) + Formula::Stat(Stat::cd);

		auto multiplier = (1.0f + totalMultiplicativeDMG) * formula + totalAdditiveDMG;
		auto dmgBonus = (1.0f + totalDMG);
		auto crit = 1.0f + totalCritRate * totalCritDMG;
		auto enemy = Formula::Constant(0.487f) * (1.0f - 0.1f);

		return _NodeRet(
			name,
			element,
			source,
			multiplier * dmgBonus * crit * enemy
		);
	}

	template<Formula::IntermediaryLike Frm>
	[[nodiscard]] consteval auto Atk(std::string_view name, Misc::AttackSource source, Frm formula) {
		return _getFormula(name, {}, source, formula, std::tuple{Formula::Constant(0.f), Formula::Constant(0.f), Formula::Constant(0.f), Formula::Constant(0.f), Formula::Constant(0.f)});
	}

	template<Formula::IntermediaryLike Frm>
	[[nodiscard]] consteval auto Atk(std::string_view name, Misc::Element element, Misc::AttackSource source, Frm formula) {
		return _getFormula(name, element, source, formula, std::tuple{Formula::Constant(0.f), Formula::Constant(0.f), Formula::Constant(0.f), Formula::Constant(0.f), Formula::Constant(0.f)});
	}

	template<Formula::IntermediaryLike Frm, class T>
	[[nodiscard]] consteval auto Atk(std::string_view name, Misc::AttackSource source, Frm formula, T modifiers) {
		return _getFormula(name, {}, source, formula, modifiers);
	}
	template<Formula::IntermediaryLike Frm, class T>
	[[nodiscard]] consteval auto Atk(std::string_view name, Misc::Element element, Misc::AttackSource source, Frm formula, T modifiers) {
		return _getFormula(name, element, source, formula, modifiers);
	}
}// namespace Node