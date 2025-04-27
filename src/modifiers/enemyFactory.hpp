#pragma once

#include "helpers.hpp"
#include "stats/sheet.hpp"
#include "type_traits"


namespace Modifiers {
	template<class T>
	struct EnemyPointerFactory {
		using TT = std::remove_cvref_t<T>;

		template<auto V>
		struct Skill {
			static constexpr auto pyro = SkillType{V, &TT::_SkillValue::pyro};
			static constexpr auto hydro = SkillType{V, &TT::_SkillValue::hydro};
			static constexpr auto cryo = SkillType{V, &TT::_SkillValue::cryo};
			static constexpr auto electro = SkillType{V, &TT::_SkillValue::electro};
			static constexpr auto dendro = SkillType{V, &TT::_SkillValue::dendro};
			static constexpr auto anemo = SkillType{V, &TT::_SkillValue::anemo};
			static constexpr auto geo = SkillType{V, &TT::_SkillValue::geo};
			static constexpr auto physical = SkillType{V, &TT::_SkillValue::physical};
		};

		static constexpr auto level = &TT::level;
		static constexpr auto DEFReduction = &TT::DEFReduction;
		static constexpr auto DEFIgnored = &TT::DEFIgnored;
		static constexpr auto resistance = Skill<&TT::resistance>();
	};

	struct EnemyNameFactory {
		template<auto member>
		struct Skill {
			static constexpr auto pyro = SheetMemberIdentifier(member, Misc::Element::pyro);
			static constexpr auto hydro = SheetMemberIdentifier(member, Misc::Element::hydro);
			static constexpr auto cryo = SheetMemberIdentifier(member, Misc::Element::cryo);
			static constexpr auto electro = SheetMemberIdentifier(member, Misc::Element::electro);
			static constexpr auto dendro = SheetMemberIdentifier(member, Misc::Element::dendro);
			static constexpr auto anemo = SheetMemberIdentifier(member, Misc::Element::anemo);
			static constexpr auto geo = SheetMemberIdentifier(member, Misc::Element::geo);
			static constexpr auto physical = SheetMemberIdentifier(member, Misc::Element::physical);
		};

		static constexpr auto level = SheetMemberIdentifier(Misc::EnemyStat::level);
		static constexpr auto DEFReduction = SheetMemberIdentifier(Misc::EnemyStat::DEFReduction);
		static constexpr auto DEFIgnored = SheetMemberIdentifier(Misc::EnemyStat::DEFIgnored);
		static constexpr auto resistance = Skill<Misc::EnemyResistances::resistance>();
	};

	template<template<auto...> class Formula, auto... Params>
	struct EnemyFactory {
		template<auto... V>
		struct _SkillValue {
			static constexpr Formula<V.pyro...> pyro{};
			static constexpr Formula<V.hydro...> hydro{};
			static constexpr Formula<V.cryo...> cryo{};
			static constexpr Formula<V.electro...> electro{};
			static constexpr Formula<V.dendro...> dendro{};
			static constexpr Formula<V.anemo...> anemo{};
			static constexpr Formula<V.geo...> geo{};
			static constexpr Formula<V.physical...> physical{};
		};

		static constexpr Formula<Params.level...> level{};
		static constexpr Formula<Params.DEFReduction...> DEFReduction{};
		static constexpr Formula<Params.DEFIgnored...> DEFIgnored{};
		static constexpr _SkillValue<Params.resistance...> resistance{};
	};

	template<class T, class Formula>
	[[nodiscard]] inline Stats::EnemySheet<T> enemyFactory(auto... params) {
		return {
			.level = Formula(params.level...),
			.DEFReduction = Formula(params.DEFReduction...),
			.DEFIgnored = Formula(params.DEFIgnored...),
			.resistance{
				.pyro = Formula(params.resistance.pyro...),
				.hydro = Formula(params.resistance.hydro...),
				.cryo = Formula(params.resistance.cryo...),
				.electro = Formula(params.resistance.electro...),
				.dendro = Formula(params.resistance.dendro...),
				.anemo = Formula(params.resistance.anemo...),
				.geo = Formula(params.resistance.geo...),
				.physical = Formula(params.resistance.physical...),
			},
		};
	}
}// namespace Modifiers