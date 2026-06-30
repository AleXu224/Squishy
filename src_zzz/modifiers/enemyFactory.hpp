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
			static constexpr auto wind = SkillType{V, &TT::_SkillValue::wind};
		};

		static constexpr auto level = &TT::level;
		static constexpr auto stunMod = &TT::stunMod;
		static constexpr auto baseDef = &TT::baseDef;
		static constexpr auto DEFReduction = &TT::DEFReduction;
		static constexpr auto DEFIgnored = &TT::DEFIgnored;
		static constexpr auto dazeRes = &TT::dazeRes;
		static constexpr auto daze = &TT::daze;
		static constexpr auto dazeTaken = &TT::dazeTaken;
		static constexpr auto resistance = Skill<&TT::resistance>();
	};

	template<class T, auto member>
	struct EnemyPointerFactoryModifier {
		using TT = std::remove_cvref_t<T>;

		struct Skill {
			static constexpr auto physical = EnemyResSkillType{member, &TT::_SkillValue::physical};
			static constexpr auto fire = EnemyResSkillType{member, &TT::_SkillValue::fire};
			static constexpr auto ice = EnemyResSkillType{member, &TT::_SkillValue::ice};
			static constexpr auto electric = EnemyResSkillType{member, &TT::_SkillValue::electric};
			static constexpr auto ether = EnemyResSkillType{member, &TT::_SkillValue::ether};
			static constexpr auto wind = EnemyResSkillType{member, &TT::_SkillValue::wind};
		};

		static constexpr auto level = EnemySkillType{member, &TT::level};
		static constexpr auto stunMod = EnemySkillType{member, &TT::stunMod};
		static constexpr auto baseDef = EnemySkillType{member, &TT::baseDef};
		static constexpr auto DEFReduction = EnemySkillType{member, &TT::DEFReduction};
		static constexpr auto DEFIgnored = EnemySkillType{member, &TT::DEFIgnored};
		static constexpr auto dazeRes = EnemySkillType{member, &TT::dazeRes};
		static constexpr auto daze = EnemySkillType{member, &TT::daze};
		static constexpr auto dazeTaken = EnemySkillType{member, &TT::dazeTaken};
		static constexpr auto resistance = Skill();
	};

	struct EnemyNameFactory {
		template<auto member>
		struct Skill {
			static constexpr auto physical = SheetMemberIdentifier(member, Misc::Attribute::physical);
			static constexpr auto fire = SheetMemberIdentifier(member, Misc::Attribute::fire);
			static constexpr auto ice = SheetMemberIdentifier(member, Misc::Attribute::ice);
			static constexpr auto electric = SheetMemberIdentifier(member, Misc::Attribute::electric);
			static constexpr auto ether = SheetMemberIdentifier(member, Misc::Attribute::ether);
			static constexpr auto wind = SheetMemberIdentifier(member, Misc::Attribute::wind);
		};

		static constexpr auto level = SheetMemberIdentifier(Misc::EnemyStat::level);
		static constexpr auto stunMod = SheetMemberIdentifier(Misc::EnemyStat::stunMod);
		static constexpr auto baseDef = SheetMemberIdentifier(Misc::EnemyStat::baseDef);
		static constexpr auto DEFReduction = SheetMemberIdentifier(Misc::EnemyStat::DEFReduction);
		static constexpr auto DEFIgnored = SheetMemberIdentifier(Misc::EnemyStat::DEFIgnored);
		static constexpr auto dazeRes = SheetMemberIdentifier(Misc::EnemyStat::dazeRes);
		static constexpr auto daze = SheetMemberIdentifier(Misc::EnemyStat::daze);
		static constexpr auto dazeTaken = SheetMemberIdentifier(Misc::EnemyStat::dazeTaken);
		static constexpr auto resistance = Skill<Misc::EnemyResistances::resistance>();
	};

	// For use in _SkillValue
	// Needs a separate one because the SheetMemberIdentifier needs a different set of parameters while used in this context
	template<auto member>
	struct EnemyNameFactoryModifier {
		template<auto member2>
		struct Skill {
			static constexpr auto physical = SheetMemberIdentifier(member, std::pair(member2, Misc::Attribute::physical));
			static constexpr auto fire = SheetMemberIdentifier(member, std::pair(member2, Misc::Attribute::fire));
			static constexpr auto ice = SheetMemberIdentifier(member, std::pair(member2, Misc::Attribute::ice));
			static constexpr auto electric = SheetMemberIdentifier(member, std::pair(member2, Misc::Attribute::electric));
			static constexpr auto ether = SheetMemberIdentifier(member, std::pair(member2, Misc::Attribute::ether));
			static constexpr auto wind = SheetMemberIdentifier(member, std::pair(member2, Misc::Attribute::wind));
		};

		static constexpr auto level = SheetMemberIdentifier(member, Misc::EnemyStat::level);
		static constexpr auto stunMod = SheetMemberIdentifier(member, Misc::EnemyStat::stunMod);
		static constexpr auto baseDef = SheetMemberIdentifier(member, Misc::EnemyStat::baseDef);
		static constexpr auto DEFReduction = SheetMemberIdentifier(member, Misc::EnemyStat::DEFReduction);
		static constexpr auto DEFIgnored = SheetMemberIdentifier(member, Misc::EnemyStat::DEFIgnored);
		static constexpr auto dazeRes = SheetMemberIdentifier(member, Misc::EnemyStat::dazeRes);
		static constexpr auto daze = SheetMemberIdentifier(member, Misc::EnemyStat::daze);
		static constexpr auto dazeTaken = SheetMemberIdentifier(member, Misc::EnemyStat::dazeTaken);
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
			static constexpr Formula<V.wind...> wind{};
		};

		static constexpr Formula<Params.level...> level{};
		static constexpr Formula<Params.stunMod...> stunMod{};
		static constexpr Formula<Params.baseDef...> baseDef{};
		static constexpr Formula<Params.DEFReduction...> DEFReduction{};
		static constexpr Formula<Params.DEFIgnored...> DEFIgnored{};
		static constexpr Formula<Params.dazeRes...> dazeRes{};
		static constexpr Formula<Params.daze...> daze{};
		static constexpr Formula<Params.dazeTaken...> dazeTaken{};
		static constexpr _SkillValue<Params.resistance...> resistance{};
	};

	template<class T, class Formula>
	[[nodiscard]] inline auto formulaFactory(auto... params) {
		if constexpr (::Formula::template FormulaConcept<Formula, typename T::RetType>) {
			return Formula({}, params...);
		} else {
			return Formula(params...);
		}
	}
	template<class T, class Formula>
	[[nodiscard]] inline Stats::EnemySheet<T> enemyFactory(auto... params) {
		return {
			.level = formulaFactory<T, Formula>(params.level...),
			.stunMod = formulaFactory<T, Formula>(params.stunMod...),
			.baseDef = formulaFactory<T, Formula>(params.baseDef...),
			.DEFReduction = formulaFactory<T, Formula>(params.DEFReduction...),
			.DEFIgnored = formulaFactory<T, Formula>(params.DEFIgnored...),
			.dazeRes = formulaFactory<T, Formula>(params.dazeRes...),
			.daze = formulaFactory<T, Formula>(params.daze...),
			.dazeTaken = formulaFactory<T, Formula>(params.dazeTaken...),
			.resistance{
				.physical = formulaFactory<T, Formula>(params.resistance.physical...),
				.fire = formulaFactory<T, Formula>(params.resistance.fire...),
				.ice = formulaFactory<T, Formula>(params.resistance.ice...),
				.electric = formulaFactory<T, Formula>(params.resistance.electric...),
				.ether = formulaFactory<T, Formula>(params.resistance.ether...),
				.wind = formulaFactory<T, Formula>(params.resistance.wind...),
			},
		};
	}

	template<class T>
	[[nodiscard]] inline Stats::EnemySheet<T> constantEnemyFactory(auto param) {
		return {
			.level = param,
			.stunMod = param,
			.baseDef = param,
			.DEFReduction = param,
			.DEFIgnored = param,
			.dazeRes = param,
			.daze = param,
			.dazeTaken = param,
			.resistance{
				.physical = param,
				.fire = param,
				.ice = param,
				.electric = param,
				.ether = param,
				.wind = param,
			},
		};
	}
}// namespace Modifiers