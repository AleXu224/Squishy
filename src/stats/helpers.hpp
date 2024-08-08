#pragma once

#include "concepts"
#include "misc/attackSource.hpp"
#include "stats/stat.hpp"
#include "type_traits"


namespace Stats {
	struct Sheet;

	template<class T, class V = float>
	struct Value;

	template<class T, class V = float>
	struct SkillValue;

	using SV = Stats::Value<Stats::Sheet>;
	using SSV = Stats::SkillValue<Stats::Sheet>;
	template<class T>
	concept SheetLike = requires(T a) {
		{ std::decay_t<T>::hp } -> std::same_as<SV &>;
		{ std::decay_t<T>::hp_ } -> std::same_as<SV &>;
		{ std::decay_t<T>::baseHp } -> std::same_as<SV &>;
		{ std::decay_t<T>::atk } -> std::same_as<SV &>;
		{ std::decay_t<T>::atk_ } -> std::same_as<SV &>;
		{ std::decay_t<T>::baseAtk } -> std::same_as<SV &>;
		{ std::decay_t<T>::additionalAtk } -> std::same_as<SV &>;
		{ std::decay_t<T>::def } -> std::same_as<SV &>;
		{ std::decay_t<T>::def_ } -> std::same_as<SV &>;
		{ std::decay_t<T>::baseDef } -> std::same_as<SV &>;
		{ std::decay_t<T>::er } -> std::same_as<SV &>;
		{ std::decay_t<T>::em } -> std::same_as<SV &>;
		{ std::decay_t<T>::cr } -> std::same_as<SV &>;
		{ std::decay_t<T>::cd } -> std::same_as<SV &>;
		{ std::decay_t<T>::hb } -> std::same_as<SV &>;

		{ std::decay_t<T>::pyro } -> std::same_as<SSV &>;
		{ std::decay_t<T>::hydro } -> std::same_as<SSV &>;
		{ std::decay_t<T>::cryo } -> std::same_as<SSV &>;
		{ std::decay_t<T>::electro } -> std::same_as<SSV &>;
		{ std::decay_t<T>::dendro } -> std::same_as<SSV &>;
		{ std::decay_t<T>::anemo } -> std::same_as<SSV &>;
		{ std::decay_t<T>::geo } -> std::same_as<SSV &>;
		{ std::decay_t<T>::physical } -> std::same_as<SSV &>;
		{ std::decay_t<T>::all } -> std::same_as<SSV &>;
		{ std::decay_t<T>::normal } -> std::same_as<SSV &>;
		{ std::decay_t<T>::charged } -> std::same_as<SSV &>;
		{ std::decay_t<T>::plunge } -> std::same_as<SSV &>;
		{ std::decay_t<T>::skill } -> std::same_as<SSV &>;
		{ std::decay_t<T>::burst } -> std::same_as<SSV &>;
	};

	template<SheetLike T>
	[[nodiscard]] std::conditional_t<std::is_const_v<T>, const SV &, SV &> fromStat(T &sheet, const Stat &stat) {
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
	[[nodiscard]] SSV &fromElement(T &sheet, const Misc::Element &element) {
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
	[[nodiscard]] SSV &fromAttackSource(T &sheet, const Misc::AttackSource &attackSource) {
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
}// namespace Stats