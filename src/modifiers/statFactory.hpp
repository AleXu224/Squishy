#pragma once

#include "type_traits"
#include <utility>
#include "helpers.hpp"

namespace Modifiers {
	template<class T>
	struct StatPointerFactory {
		using TT = std::remove_cvref_t<T>;

		template<auto V>
		struct Skill {
			static constexpr auto DMG = std::make_pair(V, &TT::_SkillValue::DMG);
			static constexpr auto additiveDMG = std::make_pair(V, &TT::_SkillValue::additiveDMG);
			static constexpr auto multiplicativeDMG = std::make_pair(V, &TT::_SkillValue::multiplicativeDMG);
			static constexpr auto critRate = std::make_pair(V, &TT::_SkillValue::critRate);
			static constexpr auto critDMG = std::make_pair(V, &TT::_SkillValue::critDMG);
		};

		static constexpr auto hp = &TT::hp;
		static constexpr auto hp_ = &TT::hp_;
		static constexpr auto baseHp = &TT::baseHp;
		static constexpr auto atk = &TT::atk;
		static constexpr auto atk_ = &TT::atk_;
		static constexpr auto baseAtk = &TT::baseAtk;
		static constexpr auto additionalAtk = &TT::additionalAtk;
		static constexpr auto def = &TT::def;
		static constexpr auto def_ = &TT::def_;
		static constexpr auto baseDef = &TT::baseDef;
		static constexpr auto er = &TT::er;
		static constexpr auto em = &TT::em;
		static constexpr auto cr = &TT::cr;
		static constexpr auto cd = &TT::cd;
		static constexpr auto hb = &TT::hb;

		static constexpr auto pyro = Skill<&TT::pyro>();
		static constexpr auto hydro = Skill<&TT::hydro>();
		static constexpr auto cryo = Skill<&TT::cryo>();
		static constexpr auto electro = Skill<&TT::electro>();
		static constexpr auto dendro = Skill<&TT::dendro>();
		static constexpr auto anemo = Skill<&TT::anemo>();
		static constexpr auto geo = Skill<&TT::geo>();
		static constexpr auto physical = Skill<&TT::physical>();
		static constexpr auto all = Skill<&TT::all>();

		static constexpr auto normal = Skill<&TT::normal>();
		static constexpr auto charged = Skill<&TT::charged>();
		static constexpr auto plunge = Skill<&TT::plunge>();
		static constexpr auto skill = Skill<&TT::skill>();
		static constexpr auto burst = Skill<&TT::burst>();

		static constexpr auto vape = Skill<&TT::vape>();
		static constexpr auto melt = Skill<&TT::melt>();

		static constexpr auto aggravate = Skill<&TT::aggravate>();
		static constexpr auto spread = Skill<&TT::spread>();

		static constexpr auto burning = Skill<&TT::burning>();
		static constexpr auto superconduct = Skill<&TT::superconduct>();
		static constexpr auto swirl = Skill<&TT::swirl>();
		static constexpr auto electroCharged = Skill<&TT::electroCharged>();
		static constexpr auto shattered = Skill<&TT::shattered>();
		static constexpr auto overloaded = Skill<&TT::overloaded>();
		static constexpr auto bloom = Skill<&TT::bloom>();
		static constexpr auto burgeon = Skill<&TT::burgeon>();
		static constexpr auto hyperbloom = Skill<&TT::hyperbloom>();
	};

	struct StatNameFactory {
		template<auto member>
		struct Skill {
			static constexpr auto DMG = Modifiers::SheetMemberIdentifier(member, Misc::SkillStat::DMG);
			static constexpr auto additiveDMG = Modifiers::SheetMemberIdentifier(member, Misc::SkillStat::additiveDMG);
			static constexpr auto multiplicativeDMG = Modifiers::SheetMemberIdentifier(member, Misc::SkillStat::multiplicativeDMG);
			static constexpr auto critRate = Modifiers::SheetMemberIdentifier(member, Misc::SkillStat::critRate);
			static constexpr auto critDMG = Modifiers::SheetMemberIdentifier(member, Misc::SkillStat::critDMG);
		};

		static constexpr auto hp = Modifiers::SheetMemberIdentifier(::Stat::hp);
		static constexpr auto hp_ = Modifiers::SheetMemberIdentifier(::Stat::hp_);
		static constexpr auto baseHp = Modifiers::SheetMemberIdentifier(::Stat::baseHp);
		static constexpr auto atk = Modifiers::SheetMemberIdentifier(::Stat::atk);
		static constexpr auto atk_ = Modifiers::SheetMemberIdentifier(::Stat::atk_);
		static constexpr auto baseAtk = Modifiers::SheetMemberIdentifier(::Stat::baseAtk);
		static constexpr auto additionalAtk = Modifiers::SheetMemberIdentifier(::Stat::additionalAtk);
		static constexpr auto def = Modifiers::SheetMemberIdentifier(::Stat::def);
		static constexpr auto def_ = Modifiers::SheetMemberIdentifier(::Stat::def_);
		static constexpr auto baseDef = Modifiers::SheetMemberIdentifier(::Stat::baseDef);
		static constexpr auto er = Modifiers::SheetMemberIdentifier(::Stat::er);
		static constexpr auto em = Modifiers::SheetMemberIdentifier(::Stat::em);
		static constexpr auto cr = Modifiers::SheetMemberIdentifier(::Stat::cr);
		static constexpr auto cd = Modifiers::SheetMemberIdentifier(::Stat::cd);
		static constexpr auto hb = Modifiers::SheetMemberIdentifier(::Stat::hb);

		static constexpr auto pyro = Skill<Misc::DamageElement::pyro>();
		static constexpr auto hydro = Skill<Misc::DamageElement::hydro>();
		static constexpr auto cryo = Skill<Misc::DamageElement::cryo>();
		static constexpr auto electro = Skill<Misc::DamageElement::electro>();
		static constexpr auto dendro = Skill<Misc::DamageElement::dendro>();
		static constexpr auto anemo = Skill<Misc::DamageElement::anemo>();
		static constexpr auto geo = Skill<Misc::DamageElement::geo>();
		static constexpr auto physical = Skill<Misc::DamageElement::physical>();
		static constexpr auto all = Skill<Misc::DamageElement::all>();

		static constexpr auto normal = Skill<Misc::AttackSource::normal>();
		static constexpr auto charged = Skill<Misc::AttackSource::charged>();
		static constexpr auto plunge = Skill<Misc::AttackSource::plunge>();
		static constexpr auto skill = Skill<Misc::AttackSource::skill>();
		static constexpr auto burst = Skill<Misc::AttackSource::burst>();

		static constexpr auto vape = Skill<Misc::Reaction::vape>();
		static constexpr auto melt = Skill<Misc::Reaction::melt>();

		static constexpr auto aggravate = Skill<Misc::Reaction::aggravate>();
		static constexpr auto spread = Skill<Misc::Reaction::spread>();

		static constexpr auto burning = Skill<Misc::Reaction::burning>();
		static constexpr auto superconduct = Skill<Misc::Reaction::superconduct>();
		static constexpr auto swirl = Skill<Misc::Reaction::swirl>();
		static constexpr auto electroCharged = Skill<Misc::Reaction::electroCharged>();
		static constexpr auto shattered = Skill<Misc::Reaction::shattered>();
		static constexpr auto overloaded = Skill<Misc::Reaction::overloaded>();
		static constexpr auto bloom = Skill<Misc::Reaction::bloom>();
		static constexpr auto burgeon = Skill<Misc::Reaction::burgeon>();
		static constexpr auto hyperbloom = Skill<Misc::Reaction::hyperbloom>();
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
		};

		static constexpr Formula<Params.hp...> hp{};
		static constexpr Formula<Params.hp_...> hp_{};
		static constexpr Formula<Params.baseHp...> baseHp{};
		static constexpr Formula<Params.atk...> atk{};
		static constexpr Formula<Params.atk_...> atk_{};
		static constexpr Formula<Params.baseAtk...> baseAtk{};
		static constexpr Formula<Params.additionalAtk...> additionalAtk{};
		static constexpr Formula<Params.def...> def{};
		static constexpr Formula<Params.def_...> def_{};
		static constexpr Formula<Params.baseDef...> baseDef{};
		static constexpr Formula<Params.er...> er{};
		static constexpr Formula<Params.em...> em{};
		static constexpr Formula<Params.cr...> cr{};
		static constexpr Formula<Params.cd...> cd{};
		static constexpr Formula<Params.hb...> hb{};

		static constexpr _SkillValue<Params.pyro...> pyro{};
		static constexpr _SkillValue<Params.hydro...> hydro{};
		static constexpr _SkillValue<Params.cryo...> cryo{};
		static constexpr _SkillValue<Params.electro...> electro{};
		static constexpr _SkillValue<Params.dendro...> dendro{};
		static constexpr _SkillValue<Params.anemo...> anemo{};
		static constexpr _SkillValue<Params.geo...> geo{};
		static constexpr _SkillValue<Params.physical...> physical{};
		static constexpr _SkillValue<Params.all...> all{};

		static constexpr _SkillValue<Params.normal...> normal{};
		static constexpr _SkillValue<Params.charged...> charged{};
		static constexpr _SkillValue<Params.plunge...> plunge{};
		static constexpr _SkillValue<Params.skill...> skill{};
		static constexpr _SkillValue<Params.burst...> burst{};

		static constexpr _SkillValue<Params.vape...> vape{};
		static constexpr _SkillValue<Params.melt...> melt{};

		static constexpr _SkillValue<Params.aggravate...> aggravate{};
		static constexpr _SkillValue<Params.spread...> spread{};

		static constexpr _SkillValue<Params.burning...> burning{};
		static constexpr _SkillValue<Params.superconduct...> superconduct{};
		static constexpr _SkillValue<Params.swirl...> swirl{};
		static constexpr _SkillValue<Params.electroCharged...> electroCharged{};
		static constexpr _SkillValue<Params.shattered...> shattered{};
		static constexpr _SkillValue<Params.overloaded...> overloaded{};
		static constexpr _SkillValue<Params.bloom...> bloom{};
		static constexpr _SkillValue<Params.burgeon...> burgeon{};
		static constexpr _SkillValue<Params.hyperbloom...> hyperbloom{};
	};


}// namespace Modifiers