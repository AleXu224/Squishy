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

	template<class T, auto member>
	struct EnemyPointerFactoryModifier {
		using TT = std::remove_cvref_t<T>;

		struct Skill {
			static constexpr auto pyro = EnemyResSkillType{member, &TT::_SkillValue::pyro};
			static constexpr auto hydro = EnemyResSkillType{member, &TT::_SkillValue::hydro};
			static constexpr auto cryo = EnemyResSkillType{member, &TT::_SkillValue::cryo};
			static constexpr auto electro = EnemyResSkillType{member, &TT::_SkillValue::electro};
			static constexpr auto dendro = EnemyResSkillType{member, &TT::_SkillValue::dendro};
			static constexpr auto anemo = EnemyResSkillType{member, &TT::_SkillValue::anemo};
			static constexpr auto geo = EnemyResSkillType{member, &TT::_SkillValue::geo};
			static constexpr auto physical = EnemyResSkillType{member, &TT::_SkillValue::physical};
		};

		static constexpr auto level = EnemySkillType{member, &TT::level};
		static constexpr auto DEFReduction = EnemySkillType{member, &TT::DEFReduction};
		static constexpr auto DEFIgnored = EnemySkillType{member, &TT::DEFIgnored};
		static constexpr auto resistance = Skill();
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

	// For use in _SkillValue
	// Needs a separate one because the SheetMemberIdentifier needs a different set of parameters while used in this context
	template<auto member>
	struct EnemyNameFactoryModifier {
		template<auto member2>
		struct Skill {
			static constexpr auto pyro = SheetMemberIdentifier(member, std::pair(member2, Misc::Element::pyro));
			static constexpr auto hydro = SheetMemberIdentifier(member, std::pair(member2, Misc::Element::hydro));
			static constexpr auto cryo = SheetMemberIdentifier(member, std::pair(member2, Misc::Element::cryo));
			static constexpr auto electro = SheetMemberIdentifier(member, std::pair(member2, Misc::Element::electro));
			static constexpr auto dendro = SheetMemberIdentifier(member, std::pair(member2, Misc::Element::dendro));
			static constexpr auto anemo = SheetMemberIdentifier(member, std::pair(member2, Misc::Element::anemo));
			static constexpr auto geo = SheetMemberIdentifier(member, std::pair(member2, Misc::Element::geo));
			static constexpr auto physical = SheetMemberIdentifier(member, std::pair(member2, Misc::Element::physical));
		};

		static constexpr auto level = SheetMemberIdentifier(member, Misc::EnemyStat::level);
		static constexpr auto DEFReduction = SheetMemberIdentifier(member, Misc::EnemyStat::DEFReduction);
		static constexpr auto DEFIgnored = SheetMemberIdentifier(member, Misc::EnemyStat::DEFIgnored);
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
			.DEFReduction = formulaFactory<T, Formula>(params.DEFReduction...),
			.DEFIgnored = formulaFactory<T, Formula>(params.DEFIgnored...),
			.resistance{
				.pyro = formulaFactory<T, Formula>(params.resistance.pyro...),
				.hydro = formulaFactory<T, Formula>(params.resistance.hydro...),
				.cryo = formulaFactory<T, Formula>(params.resistance.cryo...),
				.electro = formulaFactory<T, Formula>(params.resistance.electro...),
				.dendro = formulaFactory<T, Formula>(params.resistance.dendro...),
				.anemo = formulaFactory<T, Formula>(params.resistance.anemo...),
				.geo = formulaFactory<T, Formula>(params.resistance.geo...),
				.physical = formulaFactory<T, Formula>(params.resistance.physical...),
			},
		};
	}

	template<class T>
	[[nodiscard]] inline Stats::EnemySheet<T> constantEnemyFactory(auto param) {
		return {
			.level = param,
			.DEFReduction = param,
			.DEFIgnored = param,
			.resistance{
				.pyro = param,
				.hydro = param,
				.cryo = param,
				.electro = param,
				.dendro = param,
				.anemo = param,
				.geo = param,
				.physical = param,
			},
		};
	}
}// namespace Modifiers