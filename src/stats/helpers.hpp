#pragma once

#include "formula/node.hpp"
#include "misc/attackSource.hpp"
#include "misc/damageElement.hpp"
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
			case Stat::incHb:
				return sheet.incHb;
			case Stat::shield_:
				return sheet.shield_;
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

	template<class T, class RetType = Formula::FloatNode>
	[[nodiscard]] constexpr const RetType &fromSkillStat(T &sheet, Misc::SkillStat skillStat) {
		switch (skillStat) {
			case Misc::SkillStat::DMG:
				return sheet.DMG;
			case Misc::SkillStat::additiveDMG:
				return sheet.additiveDMG;
			case Misc::SkillStat::multiplicativeDMG:
				return sheet.multiplicativeDMG;
			case Misc::SkillStat::elevation:
				return sheet.elevation;
			case Misc::SkillStat::critRate:
				return sheet.critRate;
			case Misc::SkillStat::critDMG:
				return sheet.critDMG;
		}
		std::unreachable();
	}

	template<auto Skill, class RetType = Formula::FloatNode>
	[[nodiscard]] constexpr RetType fromSkillStat(Misc::SkillStat skillStat) {
		switch (skillStat) {
			case Misc::SkillStat::DMG:
				return Skill.DMG;
			case Misc::SkillStat::additiveDMG:
				return Skill.additiveDMG;
			case Misc::SkillStat::multiplicativeDMG:
				return Skill.multiplicativeDMG;
			case Misc::SkillStat::elevation:
				return Skill.elevation;
			case Misc::SkillStat::critRate:
				return Skill.critRate;
			case Misc::SkillStat::critDMG:
				return Skill.critDMG;
		}
		std::unreachable();
	}
	template<auto Skill>
	[[nodiscard]] constexpr auto evalSkillStat(Misc::SkillStat skillStat, const Formula::Context &ctx) {
		switch (skillStat) {
			case Misc::SkillStat::DMG:
				return Skill.DMG.eval(ctx);
			case Misc::SkillStat::additiveDMG:
				return Skill.additiveDMG.eval(ctx);
			case Misc::SkillStat::multiplicativeDMG:
				return Skill.multiplicativeDMG.eval(ctx);
			case Misc::SkillStat::elevation:
				return Skill.elevation.eval(ctx);
			case Misc::SkillStat::critRate:
				return Skill.critRate.eval(ctx);
			case Misc::SkillStat::critDMG:
				return Skill.critDMG.eval(ctx);
		}
	}
	template<auto Skill>
	[[nodiscard]] constexpr auto compileSkillStat(Misc::SkillStat skillStat, const Formula::Context &ctx) {
		switch (skillStat) {
			case Misc::SkillStat::DMG:
				return Skill.DMG.compile(ctx);
			case Misc::SkillStat::additiveDMG:
				return Skill.additiveDMG.compile(ctx);
			case Misc::SkillStat::multiplicativeDMG:
				return Skill.multiplicativeDMG.compile(ctx);
			case Misc::SkillStat::elevation:
				return Skill.elevation.compile(ctx);
			case Misc::SkillStat::critRate:
				return Skill.critRate.compile(ctx);
			case Misc::SkillStat::critDMG:
				return Skill.critDMG.compile(ctx);
		}
	}

	template<auto Sheet, class RetType = Formula::FloatNode>
	[[nodiscard]] RetType fromStat(Stat stat, Misc::SkillStat skillStat = Misc::SkillStat::DMG) {
		switch (stat) {
			case Stat::hp:
				return Sheet.hp;
			case Stat::hp_:
				return Sheet.hp_;
			case Stat::baseHp:
				return Sheet.baseHp;
			case Stat::atk:
				return Sheet.atk;
			case Stat::atk_:
				return Sheet.atk_;
			case Stat::baseAtk:
				return Sheet.baseAtk;
			case Stat::additionalAtk:
				return Sheet.additionalAtk;
			case Stat::def:
				return Sheet.def;
			case Stat::def_:
				return Sheet.def_;
			case Stat::baseDef:
				return Sheet.baseDef;
			case Stat::er:
				return Sheet.er;
			case Stat::em:
				return Sheet.em;
			case Stat::cr:
				return Sheet.cr;
			case Stat::cd:
				return Sheet.cd;
			case Stat::hb:
				return Sheet.hb;
			case Stat::incHb:
				return Sheet.incHb;
			case Stat::shield_:
				return Sheet.shield_;
			case Stat::pyroDmg:
				return fromSkillStat<Sheet.pyro, RetType>(skillStat);
			case Stat::hydroDmg:
				return fromSkillStat<Sheet.hydro, RetType>(skillStat);
			case Stat::cryoDmg:
				return fromSkillStat<Sheet.cryo, RetType>(skillStat);
			case Stat::electroDmg:
				return fromSkillStat<Sheet.electro, RetType>(skillStat);
			case Stat::dendroDmg:
				return fromSkillStat<Sheet.dendro, RetType>(skillStat);
			case Stat::anemoDmg:
				return fromSkillStat<Sheet.anemo, RetType>(skillStat);
			case Stat::geoDmg:
				return fromSkillStat<Sheet.geo, RetType>(skillStat);
			case Stat::physicalDmg:
				return fromSkillStat<Sheet.physical, RetType>(skillStat);
			case Stat::allDmg:
				return fromSkillStat<Sheet.all, RetType>(skillStat);
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
		if constexpr (reaction == Misc::Reaction::lunarCharged) return Sheet.lunarCharged;
		if constexpr (reaction == Misc::Reaction::lunarBloom) return Sheet.lunarBloom;
		if constexpr (reaction == Misc::Reaction::lunarCrystalize) return Sheet.lunarCrystalize;
	}

	template<auto Sheet, class RetType = Formula::FloatNode>
	[[nodiscard]] constexpr RetType fromReaction(Misc::Reaction reaction, Misc::SkillStat skillStat) {
		switch (reaction) {
			case Misc::Reaction::vape:
				return fromSkillStat<Sheet.vape, RetType>(skillStat);
			case Misc::Reaction::melt:
				return fromSkillStat<Sheet.melt, RetType>(skillStat);
			case Misc::Reaction::aggravate:
				return fromSkillStat<Sheet.aggravate, RetType>(skillStat);
			case Misc::Reaction::spread:
				return fromSkillStat<Sheet.spread, RetType>(skillStat);
			case Misc::Reaction::burning:
				return fromSkillStat<Sheet.burning, RetType>(skillStat);
			case Misc::Reaction::superconduct:
				return fromSkillStat<Sheet.superconduct, RetType>(skillStat);
			case Misc::Reaction::swirl:
				return fromSkillStat<Sheet.swirl, RetType>(skillStat);
			case Misc::Reaction::electroCharged:
				return fromSkillStat<Sheet.electroCharged, RetType>(skillStat);
			case Misc::Reaction::shattered:
				return fromSkillStat<Sheet.shattered, RetType>(skillStat);
			case Misc::Reaction::overloaded:
				return fromSkillStat<Sheet.overloaded, RetType>(skillStat);
			case Misc::Reaction::bloom:
				return fromSkillStat<Sheet.bloom, RetType>(skillStat);
			case Misc::Reaction::burgeon:
				return fromSkillStat<Sheet.burgeon, RetType>(skillStat);
			case Misc::Reaction::hyperbloom:
				return fromSkillStat<Sheet.hyperbloom, RetType>(skillStat);
			case Misc::Reaction::lunarCharged:
				return fromSkillStat<Sheet.lunarCharged, RetType>(skillStat);
			case Misc::Reaction::lunarBloom:
				return fromSkillStat<Sheet.lunarBloom, RetType>(skillStat);
			case Misc::Reaction::lunarCrystalize:
				return fromSkillStat<Sheet.lunarCrystalize, RetType>(skillStat);
		}
		std::unreachable();
	}

	template<SheetLike T, class RetType = Formula::FloatNode>
	[[nodiscard]] constexpr const RetType &fromReaction(T &sheet, Misc::Reaction reaction, Misc::SkillStat skillStat) {
		switch (reaction) {
			case Misc::Reaction::vape:
				return fromSkillStat(sheet.vape, skillStat);
			case Misc::Reaction::melt:
				return fromSkillStat(sheet.melt, skillStat);
			case Misc::Reaction::aggravate:
				return fromSkillStat(sheet.aggravate, skillStat);
			case Misc::Reaction::spread:
				return fromSkillStat(sheet.spread, skillStat);
			case Misc::Reaction::burning:
				return fromSkillStat(sheet.burning, skillStat);
			case Misc::Reaction::superconduct:
				return fromSkillStat(sheet.superconduct, skillStat);
			case Misc::Reaction::swirl:
				return fromSkillStat(sheet.swirl, skillStat);
			case Misc::Reaction::electroCharged:
				return fromSkillStat(sheet.electroCharged, skillStat);
			case Misc::Reaction::shattered:
				return fromSkillStat(sheet.shattered, skillStat);
			case Misc::Reaction::overloaded:
				return fromSkillStat(sheet.overloaded, skillStat);
			case Misc::Reaction::bloom:
				return fromSkillStat(sheet.bloom, skillStat);
			case Misc::Reaction::burgeon:
				return fromSkillStat(sheet.burgeon, skillStat);
			case Misc::Reaction::hyperbloom:
				return fromSkillStat(sheet.hyperbloom, skillStat);
			case Misc::Reaction::lunarCharged:
				return fromSkillStat(sheet.lunarCharged, skillStat);
			case Misc::Reaction::lunarBloom:
				return fromSkillStat(sheet.lunarBloom, skillStat);
			case Misc::Reaction::lunarCrystalize:
				return fromSkillStat(sheet.lunarCrystalize, skillStat);
		}
		std::unreachable();
	}

	template<auto Sheet, Misc::AttackSource attackSource>
	[[nodiscard]] constexpr auto fromAttackSource() {
		if constexpr (attackSource == Misc::AttackSource::normal) return Sheet.normal;
		if constexpr (attackSource == Misc::AttackSource::charged) return Sheet.charged;
		if constexpr (attackSource == Misc::AttackSource::plunge) return Sheet.plunge;
		if constexpr (attackSource == Misc::AttackSource::skill) return Sheet.skill;
		if constexpr (attackSource == Misc::AttackSource::burst) return Sheet.burst;
	}

	template<auto Sheet, class RetType = Formula::FloatNode>
	[[nodiscard]] constexpr RetType fromAttackSource(Misc::AttackSource attackSource, Misc::SkillStat skillStat) {
		switch (attackSource) {
			case Misc::AttackSource::normal:
				return fromSkillStat<Sheet.normal, RetType>(skillStat);
			case Misc::AttackSource::charged:
				return fromSkillStat<Sheet.charged, RetType>(skillStat);
			case Misc::AttackSource::plunge:
				return fromSkillStat<Sheet.plunge, RetType>(skillStat);
			case Misc::AttackSource::skill:
				return fromSkillStat<Sheet.skill, RetType>(skillStat);
			case Misc::AttackSource::burst:
				return fromSkillStat<Sheet.burst, RetType>(skillStat);
		}
		std::unreachable();
	}

	template<SheetLike T, class RetType = Formula::FloatNode>
	[[nodiscard]] constexpr RetType fromAttackSource(T &sheet, Misc::AttackSource attackSource, Misc::SkillStat skillStat) {
		switch (attackSource) {
			case Misc::AttackSource::normal:
				return fromSkillStat(sheet.normal, skillStat);
			case Misc::AttackSource::charged:
				return fromSkillStat(sheet.charged, skillStat);
			case Misc::AttackSource::plunge:
				return fromSkillStat(sheet.plunge, skillStat);
			case Misc::AttackSource::skill:
				return fromSkillStat(sheet.skill, skillStat);
			case Misc::AttackSource::burst:
				return fromSkillStat(sheet.burst, skillStat);
		}
		std::unreachable();
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

	template<SheetLike T>
	[[nodiscard]] constexpr auto &fromElement(T &sheet, Misc::Element element) {
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
		std::unreachable();
	}

	template<SheetLike T, class RetType = Formula::FloatNode>
	[[nodiscard]] constexpr const RetType &fromDamageElement(T &sheet, Misc::DamageElement element, Misc::SkillStat skillStat) {
		switch (element) {
			case Misc::DamageElement::pyro:
				return fromSkillStat(sheet.pyro, skillStat);
			case Misc::DamageElement::hydro:
				return fromSkillStat(sheet.hydro, skillStat);
			case Misc::DamageElement::cryo:
				return fromSkillStat(sheet.cryo, skillStat);
			case Misc::DamageElement::electro:
				return fromSkillStat(sheet.electro, skillStat);
			case Misc::DamageElement::dendro:
				return fromSkillStat(sheet.dendro, skillStat);
			case Misc::DamageElement::anemo:
				return fromSkillStat(sheet.anemo, skillStat);
			case Misc::DamageElement::geo:
				return fromSkillStat(sheet.geo, skillStat);
			case Misc::DamageElement::physical:
				return fromSkillStat(sheet.physical, skillStat);
			case Misc::DamageElement::all:
				return fromSkillStat(sheet.all, skillStat);
			case Misc::DamageElement::allElemental:
				return fromSkillStat(sheet.allElemental, skillStat);
		}
		std::unreachable();
	}

	template<auto Sheet, class RetType = Formula::FloatNode>
	[[nodiscard]] constexpr RetType fromDamageElement(Misc::DamageElement element, Misc::SkillStat skillStat) {
		switch (element) {
			case Misc::DamageElement::pyro:
				return fromSkillStat<Sheet.pyro, RetType>(skillStat);
			case Misc::DamageElement::hydro:
				return fromSkillStat<Sheet.hydro, RetType>(skillStat);
			case Misc::DamageElement::cryo:
				return fromSkillStat<Sheet.cryo, RetType>(skillStat);
			case Misc::DamageElement::electro:
				return fromSkillStat<Sheet.electro, RetType>(skillStat);
			case Misc::DamageElement::dendro:
				return fromSkillStat<Sheet.dendro, RetType>(skillStat);
			case Misc::DamageElement::anemo:
				return fromSkillStat<Sheet.anemo, RetType>(skillStat);
			case Misc::DamageElement::geo:
				return fromSkillStat<Sheet.geo, RetType>(skillStat);
			case Misc::DamageElement::physical:
				return fromSkillStat<Sheet.physical, RetType>(skillStat);
			case Misc::DamageElement::all:
				return fromSkillStat<Sheet.all, RetType>(skillStat);
			case Misc::DamageElement::allElemental:
				return fromSkillStat<Sheet.allElemental, RetType>(skillStat);
		}
		std::unreachable();
	}

	template<auto Skill, Misc::SkillStat skillStat>
	[[nodiscard]] constexpr auto fromSkillStat() {
		if constexpr (skillStat == Misc::SkillStat::DMG) return Skill.DMG;
		if constexpr (skillStat == Misc::SkillStat::additiveDMG) return Skill.additiveDMG;
		if constexpr (skillStat == Misc::SkillStat::multiplicativeDMG) return Skill.multiplicativeDMG;
		if constexpr (skillStat == Misc::SkillStat::elevation) return Skill.elevation;
		if constexpr (skillStat == Misc::SkillStat::critRate) return Skill.critRate;
		if constexpr (skillStat == Misc::SkillStat::critDMG) return Skill.critDMG;
	}

	template<class T, class RetType = Formula::FloatNode>
	[[nodiscard]] constexpr const RetType &fromEnemyResElement(T &sheet, Misc::Element element) {
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
		std::unreachable();
	}

	template<auto Sheet, class RetType = Formula::FloatNode>
	[[nodiscard]] constexpr RetType fromEnemyResElement(Misc::Element element) {
		switch (element) {
			case Misc::Element::pyro:
				return Sheet.pyro;
			case Misc::Element::hydro:
				return Sheet.hydro;
			case Misc::Element::cryo:
				return Sheet.cryo;
			case Misc::Element::electro:
				return Sheet.electro;
			case Misc::Element::dendro:
				return Sheet.dendro;
			case Misc::Element::anemo:
				return Sheet.anemo;
			case Misc::Element::geo:
				return Sheet.geo;
			case Misc::Element::physical:
				return Sheet.physical;
		}
		std::unreachable();
	}
	template<auto Sheet>
	[[nodiscard]] constexpr auto evalEnemyResElement(Misc::Element element, const Formula::Context &ctx) {
		switch (element) {
			case Misc::Element::pyro:
				return Sheet.pyro.eval(ctx);
			case Misc::Element::hydro:
				return Sheet.hydro.eval(ctx);
			case Misc::Element::cryo:
				return Sheet.cryo.eval(ctx);
			case Misc::Element::electro:
				return Sheet.electro.eval(ctx);
			case Misc::Element::dendro:
				return Sheet.dendro.eval(ctx);
			case Misc::Element::anemo:
				return Sheet.anemo.eval(ctx);
			case Misc::Element::geo:
				return Sheet.geo.eval(ctx);
			case Misc::Element::physical:
				return Sheet.physical.eval(ctx);
		}
		std::unreachable();
	}
	template<auto Sheet>
	[[nodiscard]] constexpr auto compileEnemyResElement(Misc::Element element, const Formula::Context &ctx) {
		switch (element) {
			case Misc::Element::pyro:
				return Sheet.pyro.compile(ctx);
			case Misc::Element::hydro:
				return Sheet.hydro.compile(ctx);
			case Misc::Element::cryo:
				return Sheet.cryo.compile(ctx);
			case Misc::Element::electro:
				return Sheet.electro.compile(ctx);
			case Misc::Element::dendro:
				return Sheet.dendro.compile(ctx);
			case Misc::Element::anemo:
				return Sheet.anemo.compile(ctx);
			case Misc::Element::geo:
				return Sheet.geo.compile(ctx);
			case Misc::Element::physical:
				return Sheet.physical.compile(ctx);
		}
		std::unreachable();
	}
}// namespace Stats
