#pragma once

#include "misc/attackSource.hpp"
#include "misc/reaction.hpp"
#include "misc/skillStat.hpp"
#include "stats/stat.hpp"
#include "type_traits"
#include "utility"


namespace Stats {
	template<class T>
	concept SheetLike = requires(T a) {
		typename std::remove_cvref_t<T>::_Value;
		{ std::remove_cvref_t<T>::hp };
		{ std::remove_cvref_t<T>::hp_ };
		{ std::remove_cvref_t<T>::baseHp };
		{ std::remove_cvref_t<T>::atk };
		{ std::remove_cvref_t<T>::atk_ };
		{ std::remove_cvref_t<T>::baseAtk };
		{ std::remove_cvref_t<T>::additionalAtk };
		{ std::remove_cvref_t<T>::def };
		{ std::remove_cvref_t<T>::def_ };
		{ std::remove_cvref_t<T>::baseDef };
		{ std::remove_cvref_t<T>::er };
		{ std::remove_cvref_t<T>::em };
		{ std::remove_cvref_t<T>::cr };
		{ std::remove_cvref_t<T>::cd };
		{ std::remove_cvref_t<T>::hb };

		{ std::remove_cvref_t<T>::pyro };
		{ std::remove_cvref_t<T>::hydro };
		{ std::remove_cvref_t<T>::cryo };
		{ std::remove_cvref_t<T>::electro };
		{ std::remove_cvref_t<T>::dendro };
		{ std::remove_cvref_t<T>::anemo };
		{ std::remove_cvref_t<T>::geo };
		{ std::remove_cvref_t<T>::physical };
		{ std::remove_cvref_t<T>::all };
		{ std::remove_cvref_t<T>::normal };
		{ std::remove_cvref_t<T>::charged };
		{ std::remove_cvref_t<T>::plunge };
		{ std::remove_cvref_t<T>::skill };
		{ std::remove_cvref_t<T>::burst };
	};
	template<class T>
	concept EnemySheetLike = requires(T a) {
		typename std::remove_cvref_t<T>::_Value;
		{ std::remove_cvref_t<T>::level };
		{ std::remove_cvref_t<T>::DEFReduction };
		{ std::remove_cvref_t<T>::DEFIgnored };

		{ std::remove_cvref_t<T>::resistance };
	};
	template<class T>
	concept TalentSheetLike = requires(T a) {
		{ std::remove_cvref_t<T>::normal };
		{ std::remove_cvref_t<T>::skill };
		{ std::remove_cvref_t<T>::burst };
	};
}// namespace Stats

namespace Stats {
	// Character
	template<SheetLike T>
	[[nodiscard]] std::conditional_t<std::is_const_v<T>, const typename T::_Value &, typename T::_Value &> fromStat(T &sheet, const Stat &stat) {
		switch (stat) {
			case Stat::hp:
				return sheet.hp;
			case Stat::hp_:
				return sheet.hp_;
			case Stat::baseHp:
				return sheet.baseHp;
			case Stat::atk:
				return sheet.atk;
			case Stat::atk_:
				return sheet.atk_;
			case Stat::baseAtk:
				return sheet.baseAtk;
			case Stat::additionalAtk:
				return sheet.additionalAtk;
			case Stat::def:
				return sheet.def;
			case Stat::def_:
				return sheet.def_;
			case Stat::baseDef:
				return sheet.baseDef;
			case Stat::er:
				return sheet.er;
			case Stat::em:
				return sheet.em;
			case Stat::cr:
				return sheet.cr;
			case Stat::cd:
				return sheet.cd;
			case Stat::hb:
				return sheet.hb;
			case Stat::pyroDmg:
				return sheet.pyro.DMG;
			case Stat::hydroDmg:
				return sheet.hydro.DMG;
			case Stat::cryoDmg:
				return sheet.cryo.DMG;
			case Stat::electroDmg:
				return sheet.electro.DMG;
			case Stat::dendroDmg:
				return sheet.dendro.DMG;
			case Stat::anemoDmg:
				return sheet.anemo.DMG;
			case Stat::geoDmg:
				return sheet.geo.DMG;
			case Stat::physicalDmg:
				return sheet.physical.DMG;
			case Stat::allDmg:
				return sheet.all.DMG;
		}
		std::unreachable();
	}

	template<auto Sheet, Misc::Reaction reaction>
	[[nodiscard]] constexpr auto fromReaction() {
		if constexpr (reaction == Misc::Reaction::vape) return Sheet.vape;
		if constexpr (reaction == Misc::Reaction::melt) return Sheet.melt;
		if constexpr (reaction == Misc::Reaction::aggravate) return Sheet.aggravate;
		if constexpr (reaction == Misc::Reaction::spread) return Sheet.spread;
		if constexpr (reaction == Misc::Reaction::burning) return Sheet.burning;
		if constexpr (reaction == Misc::Reaction::superconduct) return Sheet.superconduct;
		if constexpr (reaction == Misc::Reaction::swirl) return Sheet.swirl;
		if constexpr (reaction == Misc::Reaction::electroCharged) return Sheet.electroCharged;
		if constexpr (reaction == Misc::Reaction::shattered) return Sheet.shattered;
		if constexpr (reaction == Misc::Reaction::overloaded) return Sheet.overloaded;
		if constexpr (reaction == Misc::Reaction::bloom) return Sheet.bloom;
		if constexpr (reaction == Misc::Reaction::burgeon) return Sheet.burgeon;
		if constexpr (reaction == Misc::Reaction::hyperbloom) return Sheet.hyperbloom;
	}

	template<auto Sheet, Misc::AttackSource attackSource>
	[[nodiscard]] constexpr auto fromAttackSource() {
		if constexpr (attackSource == Misc::AttackSource::normal) return Sheet.normal;
		if constexpr (attackSource == Misc::AttackSource::charged) return Sheet.charged;
		if constexpr (attackSource == Misc::AttackSource::plunge) return Sheet.plunge;
		if constexpr (attackSource == Misc::AttackSource::skill) return Sheet.skill;
		if constexpr (attackSource == Misc::AttackSource::burst) return Sheet.burst;
	}

	template<auto Sheet, Misc::Element element>
	[[nodiscard]] constexpr auto fromElement() {
		if constexpr (element == Misc::Element::pyro) return Sheet.pyro;
		if constexpr (element == Misc::Element::hydro) return Sheet.hydro;
		if constexpr (element == Misc::Element::cryo) return Sheet.cryo;
		if constexpr (element == Misc::Element::electro) return Sheet.electro;
		if constexpr (element == Misc::Element::dendro) return Sheet.dendro;
		if constexpr (element == Misc::Element::anemo) return Sheet.anemo;
		if constexpr (element == Misc::Element::geo) return Sheet.geo;
		if constexpr (element == Misc::Element::physical) return Sheet.physical;
	}

	template<auto Skill, Misc::SkillStat skillStat>
	[[nodiscard]] constexpr auto fromSkillStat() {
		if constexpr (skillStat == Misc::SkillStat::DMG) return Skill.DMG;
		if constexpr (skillStat == Misc::SkillStat::additiveDMG) return Skill.additiveDMG;
		if constexpr (skillStat == Misc::SkillStat::multiplicativeDMG) return Skill.multiplicativeDMG;
		if constexpr (skillStat == Misc::SkillStat::critRate) return Skill.critRate;
		if constexpr (skillStat == Misc::SkillStat::critDMG) return Skill.critDMG;
	}
}// namespace Stats
