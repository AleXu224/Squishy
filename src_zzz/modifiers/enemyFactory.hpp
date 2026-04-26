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
			static constexpr auto physical = SkillType{V, &TT::_SkillValue::physical};
			static constexpr auto fire = SkillType{V, &TT::_SkillValue::fire};
			static constexpr auto ice = SkillType{V, &TT::_SkillValue::ice};
			static constexpr auto electric = SkillType{V, &TT::_SkillValue::electric};
			static constexpr auto ether = SkillType{V, &TT::_SkillValue::ether};
		};

		static constexpr auto level = &TT::level;
		static constexpr auto baseDef = &TT::baseDef;
		static constexpr auto DEFReduction = &TT::DEFReduction;
		static constexpr auto DEFIgnored = &TT::DEFIgnored;
		static constexpr auto resistance = Skill<&TT::resistance>();
	};

	struct EnemyNameFactory {
		template<auto member>
		struct Skill {
			static constexpr auto physical = SheetMemberIdentifier(member, Misc::Attribute::physical);
			static constexpr auto fire = SheetMemberIdentifier(member, Misc::Attribute::fire);
			static constexpr auto ice = SheetMemberIdentifier(member, Misc::Attribute::ice);
			static constexpr auto electric = SheetMemberIdentifier(member, Misc::Attribute::electric);
			static constexpr auto ether = SheetMemberIdentifier(member, Misc::Attribute::ether);
		};

		static constexpr auto level = SheetMemberIdentifier(Misc::EnemyStat::level);
		static constexpr auto baseDef = SheetMemberIdentifier(Misc::EnemyStat::baseDef);
		static constexpr auto DEFReduction = SheetMemberIdentifier(Misc::EnemyStat::DEFReduction);
		static constexpr auto DEFIgnored = SheetMemberIdentifier(Misc::EnemyStat::DEFIgnored);
		static constexpr auto resistance = Skill<Misc::EnemyResistances::resistance>();
	};

	template<template<auto...> class Formula, auto... Params>
	struct EnemyFactory {
		template<auto... V>
		struct _SkillValue {
			static constexpr Formula<V.physical...> physical{};
			static constexpr Formula<V.fire...> fire{};
			static constexpr Formula<V.ice...> ice{};
			static constexpr Formula<V.electric...> electric{};
			static constexpr Formula<V.ether...> ether{};
		};

		static constexpr Formula<Params.level...> level{};
		static constexpr Formula<Params.baseDef...> baseDef{};
		static constexpr Formula<Params.DEFReduction...> DEFReduction{};
		static constexpr Formula<Params.DEFIgnored...> DEFIgnored{};
		static constexpr _SkillValue<Params.resistance...> resistance{};
	};

	template<class T, class Formula>
	[[nodiscard]] inline Stats::EnemySheet<T> enemyFactory(auto... params) {
		return {
			.level = Formula({}, params.level...),
			.baseDef = Formula({}, params.baseDef...),
			.DEFReduction = Formula({}, params.DEFReduction...),
			.DEFIgnored = Formula({}, params.DEFIgnored...),
			.resistance{
				.physical = Formula({}, params.resistance.physical...),
				.fire = Formula({}, params.resistance.fire...),
				.ice = Formula({}, params.resistance.ice...),
				.electric = Formula({}, params.resistance.electric...),
				.ether = Formula({}, params.resistance.ether...),
			},
		};
	}
}// namespace Modifiers