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
	[[nodiscard]] inline auto makeFormula(const Stat &scalingStat, const Talent &talent, const std::array<float, 15> &multipiers) {
		return [multipiers, scalingStat, talent](Stats::Sheet &stats) {
			return stats.character.sheet.fromStat(scalingStat).getTotal(stats) * multipiers.at(stats.character.sheet.talents.getLevel(talent));
		};
	}

	template<class T>
	struct _NodeRet {
		std::string_view name;
		Utils::JankyOptional<Misc::Element> element;
		Misc::AttackSource source;
		T formula;
	};

	template<class T>
	[[nodiscard]] consteval auto _getTotal(Utils::JankyOptional<Misc::Element> attackElement, Misc::AttackSource atkSource, Stats::SSV Stats::CharacterSheet::*skill, Stats::SV Stats::SSV::*stat, T formula) {
		auto allStats = Formula::SkillPtr(&Stats::CharacterSheet::all, stat);
		auto elementStats = Formula::ElementStat(atkSource, attackElement, stat);
		auto skillStats = Formula::SkillPtr(skill, stat);

		return allStats +
			   elementStats +
			   skillStats +
			   formula;
	}

	template<Formula::IntermediaryLike Frm, class Tpl>
	[[nodiscard]] consteval auto _getFormula(std::string_view name, Utils::JankyOptional<Misc::Element> element, Misc::AttackSource source, Frm formula, Tpl stats) {
		auto skill = Stats::getSheetMemberByAttackSource<Stats::CharacterSheet>(source);

		auto totalDMG = _getTotal(element, source, skill, &Stats::SSV::DMG, std::get<0>(stats));
		auto totalAdditiveDMG = _getTotal(element, source, skill, &Stats::SSV::additiveDMG, std::get<1>(stats));
		auto totalMultiplicativeDMG = _getTotal(element, source, skill, &Stats::SSV::multiplicativeDMG, std::get<2>(stats));
		auto totalCritRate = Formula::Clamp(_getTotal(element, source, skill, &Stats::SSV::critRate, std::get<3>(stats)) + Formula::Stat(Stat::cr), 0.f, 1.f);
		auto totalCritDMG = _getTotal(element, source, skill, &Stats::SSV::critDMG, std::get<4>(stats)) + Formula::Stat(Stat::cd);

		auto multiplier = (1.0f + totalMultiplicativeDMG) * formula + totalAdditiveDMG;
		auto dmgBonus = (1.0f + totalDMG);
		auto crit = 1.0f + totalCritRate * totalCritDMG;
		auto enemy = Formula::Constant(0.5f) * (1.0f - 0.1f);

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