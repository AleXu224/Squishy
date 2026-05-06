#pragma once

#include "helpers.hpp"
#include "stats/sheet.hpp"
#include "type_traits"

namespace Modifiers {
	template<class T>
	struct StatPointerFactory {
		using TT = std::remove_cvref_t<T>;

		template<auto V>
		struct Skill {
			static constexpr auto DMG = SkillType{V, &TT::_SkillValue::DMG};
			static constexpr auto additiveDMG = SkillType{V, &TT::_SkillValue::additiveDMG};
			static constexpr auto multiplicativeDMG = SkillType{V, &TT::_SkillValue::multiplicativeDMG};
			static constexpr auto critRate = SkillType{V, &TT::_SkillValue::critRate};
			static constexpr auto critDMG = SkillType{V, &TT::_SkillValue::critDMG};
			static constexpr auto daze = SkillType{V, &TT::_SkillValue::daze};
		};

		static constexpr auto hp = &TT::hp;
		static constexpr auto hp_ = &TT::hp_;
		static constexpr auto atk = &TT::atk;
		static constexpr auto atk_ = &TT::atk_;
		static constexpr auto def = &TT::def;
		static constexpr auto def_ = &TT::def_;
		static constexpr auto er = &TT::er;
		static constexpr auto ap = &TT::ap;
		static constexpr auto ap_ = &TT::ap_;
		static constexpr auto am = &TT::am;
		static constexpr auto am_ = &TT::am_;
		static constexpr auto pen = &TT::pen;
		static constexpr auto penRatio = &TT::penRatio;
		static constexpr auto impact = &TT::impact;
		static constexpr auto impact_ = &TT::impact_;
		static constexpr auto cr = &TT::cr;
		static constexpr auto cd = &TT::cd;

		static constexpr auto physical = Skill<&TT::physical>();
		static constexpr auto fire = Skill<&TT::fire>();
		static constexpr auto ice = Skill<&TT::ice>();
		static constexpr auto electric = Skill<&TT::electric>();
		static constexpr auto ether = Skill<&TT::ether>();
		static constexpr auto all = Skill<&TT::all>();

		static constexpr auto basic = Skill<&TT::basic>();
		static constexpr auto dodge = Skill<&TT::dodge>();
		static constexpr auto dashAttack = Skill<&TT::dashAttack>();
		static constexpr auto dodgeCounter = Skill<&TT::dodgeCounter>();
		static constexpr auto assist = Skill<&TT::assist>();
		static constexpr auto quickAssist = Skill<&TT::quickAssist>();
		static constexpr auto defensiveAssist = Skill<&TT::defensiveAssist>();
		static constexpr auto evasiveAssist = Skill<&TT::evasiveAssist>();
		static constexpr auto assistFollowup = Skill<&TT::assistFollowup>();
		static constexpr auto special = Skill<&TT::special>();
		static constexpr auto exspecial = Skill<&TT::exspecial>();
		static constexpr auto chain = Skill<&TT::chain>();
		static constexpr auto ultimate = Skill<&TT::ultimate>();

		static constexpr auto burn = Skill<&TT::burn>();
		static constexpr auto shock = Skill<&TT::shock>();
		static constexpr auto shatter = Skill<&TT::shatter>();
		static constexpr auto assault = Skill<&TT::assault>();
		static constexpr auto corruption = Skill<&TT::corruption>();
		static constexpr auto disorder = Skill<&TT::disorder>();
		static constexpr auto burnDisorder = Skill<&TT::burnDisorder>();
		static constexpr auto shockDisorder = Skill<&TT::shockDisorder>();
		static constexpr auto frozenDisorder = Skill<&TT::frozenDisorder>();
		static constexpr auto frozenFrostDisorder = Skill<&TT::frozenFrostDisorder>();
		static constexpr auto assaultDisorder = Skill<&TT::assaultDisorder>();
		static constexpr auto corruptionDisorder = Skill<&TT::corruptionDisorder>();
		static constexpr auto abloom = Skill<&TT::abloom>();
		static constexpr auto allAnomaly = Skill<&TT::allAnomaly>();
	};

	struct StatNameFactory {
		template<auto member>
		struct Skill {
			static constexpr auto DMG = Modifiers::SheetMemberIdentifier(member, Misc::SkillStat::DMG);
			static constexpr auto additiveDMG = Modifiers::SheetMemberIdentifier(member, Misc::SkillStat::additiveDMG);
			static constexpr auto multiplicativeDMG = Modifiers::SheetMemberIdentifier(member, Misc::SkillStat::multiplicativeDMG);
			static constexpr auto critRate = Modifiers::SheetMemberIdentifier(member, Misc::SkillStat::critRate);
			static constexpr auto critDMG = Modifiers::SheetMemberIdentifier(member, Misc::SkillStat::critDMG);
			static constexpr auto daze = Modifiers::SheetMemberIdentifier(member, Misc::SkillStat::daze);
		};

		static constexpr auto hp = Modifiers::SheetMemberIdentifier(::Stat::hp);
		static constexpr auto hp_ = Modifiers::SheetMemberIdentifier(::Stat::hp_);
		static constexpr auto atk = Modifiers::SheetMemberIdentifier(::Stat::atk);
		static constexpr auto atk_ = Modifiers::SheetMemberIdentifier(::Stat::atk_);
		static constexpr auto def = Modifiers::SheetMemberIdentifier(::Stat::def);
		static constexpr auto def_ = Modifiers::SheetMemberIdentifier(::Stat::def_);
		static constexpr auto er = Modifiers::SheetMemberIdentifier(::Stat::er);
		static constexpr auto ap = Modifiers::SheetMemberIdentifier(::Stat::ap);
		static constexpr auto ap_ = Modifiers::SheetMemberIdentifier(::Stat::ap_);
		static constexpr auto am = Modifiers::SheetMemberIdentifier(::Stat::am);
		static constexpr auto am_ = Modifiers::SheetMemberIdentifier(::Stat::am_);
		static constexpr auto pen = Modifiers::SheetMemberIdentifier(::Stat::pen);
		static constexpr auto penRatio = Modifiers::SheetMemberIdentifier(::Stat::penRatio);
		static constexpr auto impact = Modifiers::SheetMemberIdentifier(::Stat::impact);
		static constexpr auto impact_ = Modifiers::SheetMemberIdentifier(::Stat::impact_);
		static constexpr auto cr = Modifiers::SheetMemberIdentifier(::Stat::cr);
		static constexpr auto cd = Modifiers::SheetMemberIdentifier(::Stat::cd);

		static constexpr auto physical = Skill<Misc::DamageAttribute::physical>();
		static constexpr auto fire = Skill<Misc::DamageAttribute::fire>();
		static constexpr auto ice = Skill<Misc::DamageAttribute::ice>();
		static constexpr auto electric = Skill<Misc::DamageAttribute::electric>();
		static constexpr auto ether = Skill<Misc::DamageAttribute::ether>();
		static constexpr auto all = Skill<Misc::DamageAttribute::all>();

		static constexpr auto basic = Skill<Misc::AttackSource::basic>();
		static constexpr auto dodge = Skill<Misc::AttackSource::dodge>();
		static constexpr auto dashAttack = Skill<Misc::AttackSource::dashAttack>();
		static constexpr auto dodgeCounter = Skill<Misc::AttackSource::dodgeCounter>();
		static constexpr auto assist = Skill<Misc::AttackSource::assist>();
		static constexpr auto quickAssist = Skill<Misc::AttackSource::quickAssist>();
		static constexpr auto defensiveAssist = Skill<Misc::AttackSource::defensiveAssist>();
		static constexpr auto evasiveAssist = Skill<Misc::AttackSource::evasiveAssist>();
		static constexpr auto assistFollowup = Skill<Misc::AttackSource::assistFollowup>();
		static constexpr auto special = Skill<Misc::AttackSource::special>();
		static constexpr auto exspecial = Skill<Misc::AttackSource::exspecial>();
		static constexpr auto chain = Skill<Misc::AttackSource::chain>();
		static constexpr auto ultimate = Skill<Misc::AttackSource::ultimate>();

		static constexpr auto burn = Skill<Misc::DamageAnomaly::burn>();
		static constexpr auto shock = Skill<Misc::DamageAnomaly::shock>();
		static constexpr auto shatter = Skill<Misc::DamageAnomaly::shatter>();
		static constexpr auto assault = Skill<Misc::DamageAnomaly::assault>();
		static constexpr auto corruption = Skill<Misc::DamageAnomaly::corruption>();
		static constexpr auto disorder = Skill<Misc::DamageAnomaly::disorder>();
		static constexpr auto burnDisorder = Skill<Misc::DamageAnomaly::burnDisorder>();
		static constexpr auto shockDisorder = Skill<Misc::DamageAnomaly::shockDisorder>();
		static constexpr auto frozenDisorder = Skill<Misc::DamageAnomaly::frozenDisorder>();
		static constexpr auto frozenFrostDisorder = Skill<Misc::DamageAnomaly::frozenFrostDisorder>();
		static constexpr auto assaultDisorder = Skill<Misc::DamageAnomaly::assaultDisorder>();
		static constexpr auto corruptionDisorder = Skill<Misc::DamageAnomaly::corruptionDisorder>();
		static constexpr auto abloom = Skill<Misc::DamageAnomaly::abloom>();
		static constexpr auto allAnomaly = Skill<Misc::DamageAnomaly::allAnomaly>();
	};

	template<template<auto...> class Formula, auto... Params>
	struct StatFactory {
		template<auto... V>
		struct _SkillValue {
			static constexpr Formula<V.DMG...> DMG{};
			static constexpr Formula<V.additiveDMG...> additiveDMG{};
			static constexpr Formula<V.multiplicativeDMG...> multiplicativeDMG{};
			static constexpr Formula<V.critRate...> critRate{};
			static constexpr Formula<V.critDMG...> critDMG{};
			static constexpr Formula<V.daze...> daze{};
		};

		static constexpr Formula<Params.hp...> hp{};
		static constexpr Formula<Params.hp_...> hp_{};
		static constexpr Formula<Params.atk...> atk{};
		static constexpr Formula<Params.atk_...> atk_{};
		static constexpr Formula<Params.def...> def{};
		static constexpr Formula<Params.def_...> def_{};
		static constexpr Formula<Params.er...> er{};
		static constexpr Formula<Params.ap...> ap{};
		static constexpr Formula<Params.ap_...> ap_{};
		static constexpr Formula<Params.am...> am{};
		static constexpr Formula<Params.am_...> am_{};
		static constexpr Formula<Params.pen...> pen{};
		static constexpr Formula<Params.penRatio...> penRatio{};
		static constexpr Formula<Params.impact...> impact{};
		static constexpr Formula<Params.impact_...> impact_{};
		static constexpr Formula<Params.cr...> cr{};
		static constexpr Formula<Params.cd...> cd{};

		static constexpr _SkillValue<Params.physical...> physical{};
		static constexpr _SkillValue<Params.fire...> fire{};
		static constexpr _SkillValue<Params.ice...> ice{};
		static constexpr _SkillValue<Params.electric...> electric{};
		static constexpr _SkillValue<Params.ether...> ether{};
		static constexpr _SkillValue<Params.all...> all{};

		static constexpr _SkillValue<Params.basic...> basic{};
		static constexpr _SkillValue<Params.dodge...> dodge{};
		static constexpr _SkillValue<Params.dashAttack...> dashAttack{};
		static constexpr _SkillValue<Params.dodgeCounter...> dodgeCounter{};
		static constexpr _SkillValue<Params.assist...> assist{};
		static constexpr _SkillValue<Params.quickAssist...> quickAssist{};
		static constexpr _SkillValue<Params.defensiveAssist...> defensiveAssist{};
		static constexpr _SkillValue<Params.evasiveAssist...> evasiveAssist{};
		static constexpr _SkillValue<Params.assistFollowup...> assistFollowup{};
		static constexpr _SkillValue<Params.special...> special{};
		static constexpr _SkillValue<Params.exspecial...> exspecial{};
		static constexpr _SkillValue<Params.chain...> chain{};
		static constexpr _SkillValue<Params.ultimate...> ultimate{};

		static constexpr _SkillValue<Params.burn...> burn{};
		static constexpr _SkillValue<Params.shock...> shock{};
		static constexpr _SkillValue<Params.shatter...> shatter{};
		static constexpr _SkillValue<Params.assault...> assault{};
		static constexpr _SkillValue<Params.corruption...> corruption{};
		static constexpr _SkillValue<Params.disorder...> disorder{};
		static constexpr _SkillValue<Params.burnDisorder...> burnDisorder{};
		static constexpr _SkillValue<Params.shockDisorder...> shockDisorder{};
		static constexpr _SkillValue<Params.frozenDisorder...> frozenDisorder{};
		static constexpr _SkillValue<Params.frozenFrostDisorder...> frozenFrostDisorder{};
		static constexpr _SkillValue<Params.assaultDisorder...> assaultDisorder{};
		static constexpr _SkillValue<Params.corruptionDisorder...> corruptionDisorder{};
		static constexpr _SkillValue<Params.abloom...> abloom{};
		static constexpr _SkillValue<Params.allAnomaly...> allAnomaly{};
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
	[[nodiscard]] inline Stats::Sheet<T>::_SkillValue statSkillValueFactory(auto... params) {
		return {
			.DMG = formulaFactory<T, Formula>(params.DMG...),
			.additiveDMG = formulaFactory<T, Formula>(params.additiveDMG...),
			.multiplicativeDMG = formulaFactory<T, Formula>(params.multiplicativeDMG...),
			.critRate = formulaFactory<T, Formula>(params.critRate...),
			.critDMG = formulaFactory<T, Formula>(params.critDMG...),
			.daze = formulaFactory<T, Formula>(params.daze...),
		};
	}

	template<class T, class Formula, class... Params>
	[[nodiscard]] inline Stats::Sheet<T> statFactory(Params... params) {
		return Stats::Sheet<T>{
			.hp = formulaFactory<T, Formula>(params.hp...),
			.hp_ = formulaFactory<T, Formula>(params.hp_...),
			.atk = formulaFactory<T, Formula>(params.atk...),
			.atk_ = formulaFactory<T, Formula>(params.atk_...),
			.def = formulaFactory<T, Formula>(params.def...),
			.def_ = formulaFactory<T, Formula>(params.def_...),
			.er = formulaFactory<T, Formula>(params.er...),
			.ap = formulaFactory<T, Formula>(params.ap...),
			.ap_ = formulaFactory<T, Formula>(params.ap_...),
			.am = formulaFactory<T, Formula>(params.am...),
			.am_ = formulaFactory<T, Formula>(params.am_...),
			.pen = formulaFactory<T, Formula>(params.pen...),
			.penRatio = formulaFactory<T, Formula>(params.penRatio...),
			.impact = formulaFactory<T, Formula>(params.impact...),
			.impact_ = formulaFactory<T, Formula>(params.impact_...),
			.cr = formulaFactory<T, Formula>(params.cr...),
			.cd = formulaFactory<T, Formula>(params.cd...),

			.physical = statSkillValueFactory<T, Formula>(params.physical...),
			.fire = statSkillValueFactory<T, Formula>(params.fire...),
			.ice = statSkillValueFactory<T, Formula>(params.ice...),
			.electric = statSkillValueFactory<T, Formula>(params.electric...),
			.ether = statSkillValueFactory<T, Formula>(params.ether...),
			.all = statSkillValueFactory<T, Formula>(params.all...),

			.basic = statSkillValueFactory<T, Formula>(params.basic...),
			.dodge = statSkillValueFactory<T, Formula>(params.dodge...),
			.dashAttack = statSkillValueFactory<T, Formula>(params.dashAttack...),
			.dodgeCounter = statSkillValueFactory<T, Formula>(params.dodgeCounter...),
			.assist = statSkillValueFactory<T, Formula>(params.assist...),
			.quickAssist = statSkillValueFactory<T, Formula>(params.quickAssist...),
			.defensiveAssist = statSkillValueFactory<T, Formula>(params.defensiveAssist...),
			.evasiveAssist = statSkillValueFactory<T, Formula>(params.evasiveAssist...),
			.assistFollowup = statSkillValueFactory<T, Formula>(params.assistFollowup...),
			.special = statSkillValueFactory<T, Formula>(params.special...),
			.exspecial = statSkillValueFactory<T, Formula>(params.exspecial...),
			.chain = statSkillValueFactory<T, Formula>(params.chain...),
			.ultimate = statSkillValueFactory<T, Formula>(params.ultimate...),

			.burn = statSkillValueFactory<T, Formula>(params.burn...),
			.shock = statSkillValueFactory<T, Formula>(params.shock...),
			.shatter = statSkillValueFactory<T, Formula>(params.shatter...),
			.assault = statSkillValueFactory<T, Formula>(params.assault...),
			.corruption = statSkillValueFactory<T, Formula>(params.corruption...),
			.disorder = statSkillValueFactory<T, Formula>(params.disorder...),
			.burnDisorder = statSkillValueFactory<T, Formula>(params.burnDisorder...),
			.shockDisorder = statSkillValueFactory<T, Formula>(params.shockDisorder...),
			.frozenDisorder = statSkillValueFactory<T, Formula>(params.frozenDisorder...),
			.frozenFrostDisorder = statSkillValueFactory<T, Formula>(params.frozenFrostDisorder...),
			.assaultDisorder = statSkillValueFactory<T, Formula>(params.assaultDisorder...),
			.corruptionDisorder = statSkillValueFactory<T, Formula>(params.corruptionDisorder...),
			.abloom = statSkillValueFactory<T, Formula>(params.abloom...),
			.allAnomaly = statSkillValueFactory<T, Formula>(params.allAnomaly...),
		};
	}


}// namespace Modifiers