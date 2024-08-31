#pragma once

#include "functional"
#include "misc/attackSource.hpp"
#include "ranges"
#include "stats/stat.hpp"
#include "type_traits"
#include "utility"
#include "utils.hpp"


namespace Stats {
	template<class P1, size_t Size>
	struct Value;

	template<class T>
	struct SkillValue;
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
}// namespace Stats

namespace Formula {
	template<class T, class U>
	struct SkillPtr;
	template<class T, class U>
	struct StatPtr;
}// namespace Formula

namespace Stats {
	// Values
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

	template<SheetLike T>
	[[nodiscard]] consteval auto getSheetValuesMembers() {
		return std::array{
			&T::hp,
			&T::hp_,
			&T::baseHp,
			&T::atk,
			&T::atk_,
			&T::baseAtk,
			&T::additionalAtk,
			&T::def,
			&T::def_,
			&T::baseDef,
			&T::er,
			&T::em,
			&T::cr,
			&T::cd,
			&T::hb,
		};
	}

	template<SheetLike T>
	[[nodiscard]] constexpr bool isSheetMemberPercentage(typename T::_Value T:: *member) {
		if (member == &T::hp_) return true;
		if (member == &T::atk_) return true;
		if (member == &T::def_) return true;
		if (member == &T::er) return true;
		if (member == &T::cr) return true;
		if (member == &T::cd) return true;
		if (member == &T::hb) return true;
		return false;
	}

	template<SheetLike T>
	[[nodiscard]] constexpr Stat getSheetMemberStat(typename T::_Value T:: *member) {
		if (member == &T::hp) return Stat::hp;
		if (member == &T::hp_) return Stat::hp_;
		if (member == &T::baseHp) return Stat::baseHp;
		if (member == &T::atk) return Stat::atk;
		if (member == &T::atk_) return Stat::atk_;
		if (member == &T::baseAtk) return Stat::baseAtk;
		if (member == &T::additionalAtk) return Stat::additionalAtk;
		if (member == &T::def) return Stat::def;
		if (member == &T::def_) return Stat::def_;
		if (member == &T::baseDef) return Stat::baseDef;
		if (member == &T::er) return Stat::er;
		if (member == &T::em) return Stat::em;
		if (member == &T::cr) return Stat::cr;
		if (member == &T::cd) return Stat::cd;
		if (member == &T::hb) return Stat::hb;

		std::unreachable();
	}

	// Enemy
	template<EnemySheetLike T>
	[[nodiscard]] consteval auto getSheetValuesMembers() {
		return std::array{
			&T::level,
			&T::DEFReduction,
			&T::DEFIgnored,
		};
	}

	template<EnemySheetLike T>
	[[nodiscard]] constexpr bool isSheetMemberPercentage(typename T::_Value T:: *member) {
		if (member == &T::DEFReduction) return true;
		if (member == &T::DEFIgnored) return true;
		return false;
	}

	// Skills
	// Character
	template<SheetLike T>
	[[nodiscard]] constexpr auto getSheetMemberByElement(Misc::Element element) {
		switch (element) {
			case Misc::Element::pyro:
				return &T::pyro;
			case Misc::Element::hydro:
				return &T::hydro;
			case Misc::Element::cryo:
				return &T::cryo;
			case Misc::Element::electro:
				return &T::electro;
			case Misc::Element::dendro:
				return &T::dendro;
			case Misc::Element::anemo:
				return &T::anemo;
			case Misc::Element::geo:
				return &T::geo;
			case Misc::Element::physical:
				return &T::physical;
		}
		std::unreachable();
	}

	template<SheetLike T>
	[[nodiscard]] SkillValue<typename T::_Value> &fromElement(T &sheet, const Misc::Element &element) {
		switch (element) {
			case Misc::Element::pyro:
				return sheet.pyro;
			case Misc::Element::hydro:
				return sheet.hydro;
			case Misc::Element::cryo:
				return sheet.cryo;
			case Misc::Element::electro:
				return sheet.electro;
			case Misc::Element::dendro:
				return sheet.dendro;
			case Misc::Element::anemo:
				return sheet.anemo;
			case Misc::Element::geo:
				return sheet.geo;
			case Misc::Element::physical:
				return sheet.physical;
		}
	}

	template<SheetLike T>
	[[nodiscard]] consteval auto getSheetElementsMembers() {
		return std::array{
			&T::pyro,
			&T::hydro,
			&T::cryo,
			&T::electro,
			&T::dendro,
			&T::anemo,
			&T::geo,
			&T::physical,
			&T::all,
		};
	}

	template<SheetLike T>
	[[nodiscard]] constexpr auto getSheetMemberByAttackSource(Misc::AttackSource attackSource) {
		switch (attackSource) {
			case Misc::AttackSource::normal:
				return &T::normal;
			case Misc::AttackSource::charged:
				return &T::charged;
			case Misc::AttackSource::plunge:
				return &T::plunge;
			case Misc::AttackSource::skill:
				return &T::skill;
			case Misc::AttackSource::burst:
				return &T::burst;
		}
	}

	template<SheetLike T>
	[[nodiscard]] SkillValue<typename T::_Value> &fromAttackSource(T &sheet, const Misc::AttackSource &attackSource) {
		switch (attackSource) {
			case Misc::AttackSource::normal:
				return sheet.normal;
			case Misc::AttackSource::charged:
				return sheet.charged;
			case Misc::AttackSource::plunge:
				return sheet.plunge;
			case Misc::AttackSource::skill:
				return sheet.skill;
			case Misc::AttackSource::burst:
				return sheet.burst;
		}
	}

	template<SheetLike T>
	[[nodiscard]] consteval auto getSheetAttackSourceMembers() {
		return std::array{
			&T::normal,
			&T::charged,
			&T::plunge,
			&T::skill,
			&T::burst,
		};
	}

	template<SheetLike T>
	[[nodiscard]] consteval auto getSheetSkillsMembers() {
		constexpr auto elements = getSheetElementsMembers<T>();
		constexpr auto attackSources = getSheetAttackSourceMembers<T>();

		return squi::utils::mergeRanges<typename T::_SkillValue T:: *>(elements, attackSources);
	}

	// Enemy
	template<EnemySheetLike T>
	[[nodiscard]] consteval auto getEnemySheetResistances() {
		return std::array{
			&T::resistance,
		};
	}

	template<EnemySheetLike T>
	[[nodiscard]] consteval auto getSheetSkillsMembers() {
		constexpr auto resistances = getEnemySheetResistances<T>();

		return squi::utils::mergeRanges<typename T::_SkillValue T:: *>(resistances);
	}

	// Containers
	template<class T, class U>
	struct SheetValueMember {
		T::_Value T:: *stat{};
		T U:: *location{};

		consteval auto makeFormula() const {
			return Formula::StatPtr<T, U>(location, stat);
		}
	};
	template<class T, class U>
	struct SheetSkillMember {
		T::_SkillValue T:: *skill{};
		T::_Value T::_SkillValue:: *stat{};
		T U:: *location{};

		consteval auto makeFormula() const {
			return Formula::SkillPtr<T, U>(location, skill, stat);
		}
	};

	// All members
	// Character
	template<SheetLike T, class U>
	[[nodiscard]] consteval inline auto getSheetAllMembers(T U:: *location) {
		return std::tuple{
			squi::utils::evalRange(std::views::transform(
				getSheetValuesMembers<T>(),
				[location](auto &&val) {
					return SheetValueMember<T, U>{
						.stat = val,
						.location = location,
					};
				}
			)),
			squi::utils::evalRange(std::views::transform(
				std::views::cartesian_product(getSheetSkillsMembers<T>(), T::_SkillValue::getMembers()),
				[location](auto &&val) {
					return SheetSkillMember<T, U>{
						.skill = std::get<0>(val),
						.stat = std::get<1>(val),
						.location = location,
					};
				}
			)),
		};
	}
	// Enemy
	template<class T, class U>
	[[nodiscard]] consteval inline auto getEnemySheetAllMembers(T U:: *location) {
		return std::tuple{
			squi::utils::evalRange(std::views::transform(
				getSheetValuesMembers<T>(),
				[location](auto &&val) {
					return SheetValueMember<T, U>{
						.stat = val,
						.location = location,
					};
				}
			)),
			squi::utils::evalRange(std::views::transform(
				std::views::cartesian_product(getSheetSkillsMembers<T>(), T::_SkillValue::getMembers()),
				[location](auto &&val) {
					return SheetSkillMember<T, U>{
						.skill = std::get<0>(val),
						.stat = std::get<1>(val),
						.location = location,
					};
				}
			)),
		};
	}

	// Views
	template<class T>
	[[nodiscard]] inline auto allSheetValuesView(T &&sheet) {
		using TT = std::remove_cvref_t<T>;

		return std::views::transform(
			Stats::getSheetValuesMembers<TT>(),
			[&sheet](auto &&val) {
				return std::ref(std::invoke(val, sheet));
			}
		);
	}
	template<class T>
	[[nodiscard]] inline auto allSheetSkillsView(T &&sheet) {
		using TT = std::remove_cvref_t<T>;

		return std::views::transform(
			std::views::cartesian_product(
				Stats::getSheetSkillsMembers<TT>(),
				TT::_SkillValue::getMembers()
			),
			[&sheet](auto &&val) {
				return std::ref(std::invoke(std::get<1>(val), std::invoke(std::get<0>(val), sheet)));
			}
		);
	}

	template<class T, class U>
	inline void setupModifiers(T &&newMods, U &&stats, size_t index) {
		for (auto [stat, statCharacter]: std::views::zip(
				 Stats::allSheetValuesView(newMods),
				 Stats::allSheetValuesView(stats)
			 )) {
			statCharacter.get().modifiers.at(index) = stat.get();
		}
		for (auto [stat, statCharacter]: std::views::zip(
				 Stats::allSheetSkillsView(newMods),
				 Stats::allSheetSkillsView(stats)
			 )) {
			statCharacter.get().modifiers.at(index) = stat.get();
		}
	}
}// namespace Stats

namespace Utils {
	template<Stats::SheetLike T, class U>
	constexpr std::string Stringify(typename T::_SkillValue T:: *skill, typename T::_Value U:: *stat) {
		std::string_view prefix = [&]() {
			if (skill == &T::pyro) return "Pyro ";
			if (skill == &T::hydro) return "Hydro ";
			if (skill == &T::cryo) return "Cryo ";
			if (skill == &T::electro) return "Electro ";
			if (skill == &T::dendro) return "Dendro ";
			if (skill == &T::anemo) return "Anemo ";
			if (skill == &T::geo) return "Geo ";
			if (skill == &T::physical) return "Physical ";
			if (skill == &T::all) return "All ";
			if (skill == &T::normal) return "Normal Attack ";
			if (skill == &T::charged) return "Charged Attack ";
			if (skill == &T::plunge) return "Plunge Attack ";
			if (skill == &T::skill) return "Elemental Skill ";
			if (skill == &T::burst) return "Elemental Burst ";
			std::unreachable();
		}();

		std::string_view suffix = [&]() {
			if (stat == &U::DMG) return "DMG%";
			if (stat == &U::additiveDMG) return "Additive DMG";
			if (stat == &U::multiplicativeDMG) return "Multiplicative DMG";
			if (stat == &U::critRate) return "Crit Rate";
			if (stat == &U::critDMG) return "Crit DMG";
			std::unreachable();
		}();

		return std::string(prefix) + std::string(suffix);
	}
	template<Stats::EnemySheetLike T, class U>
	constexpr std::string Stringify(typename T::_SkillValue T:: *skill, typename T::_Value U:: *stat) {
		std::string_view suffix = [&]() {
			if (skill == &T::resistance) return "RES%";
			std::unreachable();
		}();

		std::string_view prefix = [&]() {
			if (stat == &U::pyro) return "Pyro ";
			if (stat == &U::hydro) return "Hydro ";
			if (stat == &U::cryo) return "Cryo ";
			if (stat == &U::electro) return "Electro ";
			if (stat == &U::dendro) return "Dendro ";
			if (stat == &U::anemo) return "Anemo ";
			if (stat == &U::geo) return "Geo ";
			if (stat == &U::physical) return "Physical ";
			std::unreachable();
		}();

		return std::string(prefix) + std::string(suffix);
	}
	template<Stats::SheetLike T, class U>
	constexpr std::string Stringify(typename T::_Value U:: *stat) {
		if (stat == &U::hp) return "HP";
		if (stat == &U::hp_) return "HP%";
		if (stat == &U::baseHp) return "Base HP";
		if (stat == &U::atk) return "ATK";
		if (stat == &U::atk_) return "ATK%";
		if (stat == &U::baseAtk) return "Base ATK";
		if (stat == &U::additionalAtk) return "Additional ATK";
		if (stat == &U::def) return "DEF";
		if (stat == &U::def_) return "DEF%";
		if (stat == &U::baseDef) return "Base DEF";
		if (stat == &U::er) return "Energy Recharge";
		if (stat == &U::em) return "Elemental Mastery";
		if (stat == &U::cr) return "Crit Rate";
		if (stat == &U::cd) return "Crit DMG";
		if (stat == &U::hb) return "Healing Bonus";
	}
	template<Stats::EnemySheetLike T, class U>
	constexpr std::string Stringify(typename T::_Value U:: *stat) {
		if (stat == &U::level) return "Level";
		if (stat == &U::defense) return "Defense";
	}
}// namespace Utils
