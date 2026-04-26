#pragma once

#include "formula/node.hpp"
#include "misc/anomaly.hpp"
#include "misc/attackSource.hpp"
#include "misc/attribute.hpp"
#include "misc/damageAttribute.hpp"
#include "misc/skillStat.hpp"
#include "stat.hpp"
// #include "misc/attackSource.hpp"
// #include "misc/damageElement.hpp"
// #include "misc/reaction.hpp"
// #include "misc/skillStat.hpp"
// #include "stats/stat.hpp"
// #include "type_traits"
// #include "utility"


namespace Stats {
	template<class T>
	concept SheetLike = requires(T a) {
		typename std::remove_cvref_t<T>::_Value;
		{ std::remove_cvref_t<T>::hp };
		{ std::remove_cvref_t<T>::hp_ };
		{ std::remove_cvref_t<T>::atk };
		{ std::remove_cvref_t<T>::atk_ };
		{ std::remove_cvref_t<T>::def };
		{ std::remove_cvref_t<T>::def_ };
		{ std::remove_cvref_t<T>::er };
		{ std::remove_cvref_t<T>::ap };
		{ std::remove_cvref_t<T>::am };
		{ std::remove_cvref_t<T>::pen };
		{ std::remove_cvref_t<T>::penRatio };
		{ std::remove_cvref_t<T>::impact };
		{ std::remove_cvref_t<T>::cr };
		{ std::remove_cvref_t<T>::cd };

		{ std::remove_cvref_t<T>::physical };
		{ std::remove_cvref_t<T>::fire };
		{ std::remove_cvref_t<T>::ice };
		{ std::remove_cvref_t<T>::electric };
		{ std::remove_cvref_t<T>::ether };
		{ std::remove_cvref_t<T>::all };
	};
	// template<class T>
	// concept EnemySheetLike = requires(T a) {
	// 	typename std::remove_cvref_t<T>::_Value;
	// 	{ std::remove_cvref_t<T>::level };
	// 	{ std::remove_cvref_t<T>::baseDef };
	// 	{ std::remove_cvref_t<T>::DEFReduction };
	// 	{ std::remove_cvref_t<T>::DEFIgnored };

	// 	{ std::remove_cvref_t<T>::resistance };
	// };
	// template<class T>
	// concept SkillSheetLike = requires(T a) {
	// 	{ std::remove_cvref_t<T>::normal };
	// 	{ std::remove_cvref_t<T>::skill };
	// 	{ std::remove_cvref_t<T>::burst };
	// };
}// namespace Stats

namespace Stats {
	// Agent
	template<SheetLike T>
	[[nodiscard]] std::conditional_t<std::is_const_v<T>, const typename T::_Value &, typename T::_Value &> fromStat(T &sheet, const Stat &stat) {
		switch (stat) {
			case Stat::hp:
				return sheet.hp;
			case Stat::hp_:
				return sheet.hp_;
			case Stat::atk:
				return sheet.atk;
			case Stat::atk_:
				return sheet.atk_;
			case Stat::def:
				return sheet.def;
			case Stat::def_:
				return sheet.def_;
			case Stat::er:
				return sheet.er;
			case Stat::ap:
				return sheet.ap;
			case Stat::ap_:
				return sheet.ap_;
			case Stat::am:
				return sheet.am;
			case Stat::am_:
				return sheet.am_;
			case Stat::cr:
				return sheet.cr;
			case Stat::cd:
				return sheet.cd;
			case Stat::pen:
				return sheet.pen;
			case Stat::penRatio:
				return sheet.penRatio;
			case Stat::impact:
				return sheet.impact;
			case Stat::impact_:
				return sheet.impact_;
			case Stat::physicalDmg:
				return sheet.physical.DMG;
			case Stat::fireDmg:
				return sheet.fire.DMG;
			case Stat::iceDmg:
				return sheet.ice.DMG;
			case Stat::electricDmg:
				return sheet.electric.DMG;
			case Stat::etherDmg:
				return sheet.ether.DMG;
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
			case Misc::SkillStat::critRate:
				return Skill.critRate;
			case Misc::SkillStat::critDMG:
				return Skill.critDMG;
		}
		std::unreachable();
	}
	// template<auto Skill>
	// [[nodiscard]] constexpr auto evalSkillStat(Misc::SkillStat skillStat, const Formula::Context &ctx) {
	// 	switch (skillStat) {
	// 		case Misc::SkillStat::DMG:
	// 			return Skill.DMG.eval(ctx);
	// 		case Misc::SkillStat::additiveDMG:
	// 			return Skill.additiveDMG.eval(ctx);
	// 		case Misc::SkillStat::multiplicativeDMG:
	// 			return Skill.multiplicativeDMG.eval(ctx);
	// 		case Misc::SkillStat::elevation:
	// 			return Skill.elevation.eval(ctx);
	// 		case Misc::SkillStat::critRate:
	// 			return Skill.critRate.eval(ctx);
	// 		case Misc::SkillStat::critDMG:
	// 			return Skill.critDMG.eval(ctx);
	// 	}
	// }
	// template<auto Skill>
	// [[nodiscard]] constexpr auto compileSkillStat(Misc::SkillStat skillStat, const Formula::Context &ctx) {
	// 	switch (skillStat) {
	// 		case Misc::SkillStat::DMG:
	// 			return Skill.DMG.compile(ctx);
	// 		case Misc::SkillStat::additiveDMG:
	// 			return Skill.additiveDMG.compile(ctx);
	// 		case Misc::SkillStat::multiplicativeDMG:
	// 			return Skill.multiplicativeDMG.compile(ctx);
	// 		case Misc::SkillStat::elevation:
	// 			return Skill.elevation.compile(ctx);
	// 		case Misc::SkillStat::critRate:
	// 			return Skill.critRate.compile(ctx);
	// 		case Misc::SkillStat::critDMG:
	// 			return Skill.critDMG.compile(ctx);
	// 	}
	// }

	template<auto Sheet, class RetType = Formula::FloatNode>
	[[nodiscard]] RetType fromStat(Stat stat, Misc::SkillStat skillStat = Misc::SkillStat::DMG) {
		switch (stat) {
			case Stat::hp:
				return Sheet.hp;
			case Stat::hp_:
				return Sheet.hp_;
			case Stat::atk:
				return Sheet.atk;
			case Stat::atk_:
				return Sheet.atk_;
			case Stat::def:
				return Sheet.def;
			case Stat::def_:
				return Sheet.def_;
			case Stat::er:
				return Sheet.er;
			case Stat::ap:
				return Sheet.ap;
			case Stat::ap_:
				return Sheet.ap_;
			case Stat::am:
				return Sheet.am;
			case Stat::am_:
				return Sheet.am_;
			case Stat::cr:
				return Sheet.cr;
			case Stat::cd:
				return Sheet.cd;
			case Stat::pen:
				return Sheet.pen;
			case Stat::penRatio:
				return Sheet.penRatio;
			case Stat::impact:
				return Sheet.impact;
			case Stat::impact_:
				return Sheet.impact_;
			case Stat::physicalDmg:
				return fromSkillStat<Sheet.physical, RetType>(skillStat);
			case Stat::fireDmg:
				return fromSkillStat<Sheet.fire, RetType>(skillStat);
			case Stat::iceDmg:
				return fromSkillStat<Sheet.ice, RetType>(skillStat);
			case Stat::electricDmg:
				return fromSkillStat<Sheet.electric, RetType>(skillStat);
			case Stat::etherDmg:
				return fromSkillStat<Sheet.ether, RetType>(skillStat);
			case Stat::allDmg:
				return fromSkillStat<Sheet.all, RetType>(skillStat);
		}
		std::unreachable();
	}

	template<auto Sheet, Misc::DamageAnomaly anomaly>
	[[nodiscard]] constexpr auto fromAnomaly() {
		if constexpr (anomaly == Misc::DamageAnomaly::burn) return Sheet.burn;
		if constexpr (anomaly == Misc::DamageAnomaly::shock) return Sheet.shock;
		if constexpr (anomaly == Misc::DamageAnomaly::shatter) return Sheet.shatter;
		if constexpr (anomaly == Misc::DamageAnomaly::assault) return Sheet.assault;
		if constexpr (anomaly == Misc::DamageAnomaly::corruption) return Sheet.corruption;
		if constexpr (anomaly == Misc::DamageAnomaly::disorder) return Sheet.disorder;
		if constexpr (anomaly == Misc::DamageAnomaly::burnDisorder) return Sheet.burnDisorder;
		if constexpr (anomaly == Misc::DamageAnomaly::shockDisorder) return Sheet.shockDisorder;
		if constexpr (anomaly == Misc::DamageAnomaly::frozenDisorder) return Sheet.frozenDisorder;
		if constexpr (anomaly == Misc::DamageAnomaly::frozenFrostDisorder) return Sheet.frozenFrostDisorder;
		if constexpr (anomaly == Misc::DamageAnomaly::assaultDisorder) return Sheet.assaultDisorder;
		if constexpr (anomaly == Misc::DamageAnomaly::corruptionDisorder) return Sheet.corruptionDisorder;
		if constexpr (anomaly == Misc::DamageAnomaly::abloom) return Sheet.abloom;
		if constexpr (anomaly == Misc::DamageAnomaly::allAnomaly) return Sheet.allAnomaly;
	}

	template<auto Sheet, class RetType = Formula::FloatNode>
	[[nodiscard]] constexpr RetType fromAnomaly(Misc::DamageAnomaly anomaly, Misc::SkillStat skillStat) {
		switch (anomaly) {
			case Misc::DamageAnomaly::burn:
				return fromSkillStat<Sheet.burn, RetType>(skillStat);
			case Misc::DamageAnomaly::shock:
				return fromSkillStat<Sheet.shock, RetType>(skillStat);
			case Misc::DamageAnomaly::shatter:
				return fromSkillStat<Sheet.shatter, RetType>(skillStat);
			case Misc::DamageAnomaly::assault:
				return fromSkillStat<Sheet.assault, RetType>(skillStat);
			case Misc::DamageAnomaly::corruption:
				return fromSkillStat<Sheet.corruption, RetType>(skillStat);
			case Misc::DamageAnomaly::disorder:
				return fromSkillStat<Sheet.disorder, RetType>(skillStat);
			case Misc::DamageAnomaly::burnDisorder:
				return fromSkillStat<Sheet.burnDisorder, RetType>(skillStat);
			case Misc::DamageAnomaly::shockDisorder:
				return fromSkillStat<Sheet.shockDisorder, RetType>(skillStat);
			case Misc::DamageAnomaly::frozenDisorder:
				return fromSkillStat<Sheet.frozenDisorder, RetType>(skillStat);
			case Misc::DamageAnomaly::frozenFrostDisorder:
				return fromSkillStat<Sheet.frozenFrostDisorder, RetType>(skillStat);
			case Misc::DamageAnomaly::assaultDisorder:
				return fromSkillStat<Sheet.assaultDisorder, RetType>(skillStat);
			case Misc::DamageAnomaly::corruptionDisorder:
				return fromSkillStat<Sheet.corruptionDisorder, RetType>(skillStat);
			case Misc::DamageAnomaly::abloom:
				return fromSkillStat<Sheet.abloom, RetType>(skillStat);
			case Misc::DamageAnomaly::allAnomaly:
				return fromSkillStat<Sheet.allAnomaly, RetType>(skillStat);
		}
		std::unreachable();
	}

	template<SheetLike T, class RetType = Formula::FloatNode>
	[[nodiscard]] constexpr const RetType &fromAnomaly(T &sheet, Misc::DamageAnomaly anomaly, Misc::SkillStat skillStat) {
		switch (anomaly) {
			case Misc::DamageAnomaly::burn:
				return fromSkillStat(sheet.burn, skillStat);
			case Misc::DamageAnomaly::shock:
				return fromSkillStat(sheet.shock, skillStat);
			case Misc::DamageAnomaly::shatter:
				return fromSkillStat(sheet.shatter, skillStat);
			case Misc::DamageAnomaly::assault:
				return fromSkillStat(sheet.assault, skillStat);
			case Misc::DamageAnomaly::corruption:
				return fromSkillStat(sheet.corruption, skillStat);
			case Misc::DamageAnomaly::disorder:
				return fromSkillStat(sheet.disorder, skillStat);
			case Misc::DamageAnomaly::burnDisorder:
				return fromSkillStat(sheet.burnDisorder, skillStat);
			case Misc::DamageAnomaly::shockDisorder:
				return fromSkillStat(sheet.shockDisorder, skillStat);
			case Misc::DamageAnomaly::frozenDisorder:
				return fromSkillStat(sheet.frozenDisorder, skillStat);
			case Misc::DamageAnomaly::frozenFrostDisorder:
				return fromSkillStat(sheet.frozenFrostDisorder, skillStat);
			case Misc::DamageAnomaly::assaultDisorder:
				return fromSkillStat(sheet.assaultDisorder, skillStat);
			case Misc::DamageAnomaly::corruptionDisorder:
				return fromSkillStat(sheet.corruptionDisorder, skillStat);
			case Misc::DamageAnomaly::abloom:
				return fromSkillStat(sheet.abloom, skillStat);
			case Misc::DamageAnomaly::allAnomaly:
				return fromSkillStat(sheet.allAnomaly, skillStat);
		}
		std::unreachable();
	}
	[[nodiscard]] constexpr auto &&fromAnomaly(auto &&sheet, Misc::DamageAnomaly anomaly) {
		switch (anomaly) {
			case Misc::DamageAnomaly::burn:
				return sheet.burn;
			case Misc::DamageAnomaly::shock:
				return sheet.shock;
			case Misc::DamageAnomaly::shatter:
				return sheet.shatter;
			case Misc::DamageAnomaly::assault:
				return sheet.assault;
			case Misc::DamageAnomaly::corruption:
				return sheet.corruption;
			case Misc::DamageAnomaly::disorder:
				return sheet.disorder;
			case Misc::DamageAnomaly::burnDisorder:
				return sheet.burnDisorder;
			case Misc::DamageAnomaly::shockDisorder:
				return sheet.shockDisorder;
			case Misc::DamageAnomaly::frozenDisorder:
				return sheet.frozenDisorder;
			case Misc::DamageAnomaly::frozenFrostDisorder:
				return sheet.frozenFrostDisorder;
			case Misc::DamageAnomaly::assaultDisorder:
				return sheet.assaultDisorder;
			case Misc::DamageAnomaly::corruptionDisorder:
				return sheet.corruptionDisorder;
			case Misc::DamageAnomaly::abloom:
				return sheet.abloom;
			case Misc::DamageAnomaly::allAnomaly:
				return sheet.allAnomaly;
		}
		std::unreachable();
	}

	// template<auto Sheet, Misc::AttackSource attackSource>
	// [[nodiscard]] constexpr auto fromAttackSource() {
	// 	if constexpr (attackSource == Misc::AttackSource::normal) return Sheet.normal;
	// 	if constexpr (attackSource == Misc::AttackSource::charged) return Sheet.charged;
	// 	if constexpr (attackSource == Misc::AttackSource::plunge) return Sheet.plunge;
	// 	if constexpr (attackSource == Misc::AttackSource::skill) return Sheet.skill;
	// 	if constexpr (attackSource == Misc::AttackSource::burst) return Sheet.burst;
	// }

	template<auto Sheet, class RetType = Formula::FloatNode>
	[[nodiscard]] constexpr RetType fromAttackSource(Misc::AttackSource attackSource, Misc::SkillStat skillStat) {
		switch (attackSource) {
			case Misc::AttackSource::basic:
				return fromSkillStat<Sheet.basic, RetType>(skillStat);
			case Misc::AttackSource::dodge:
				return fromSkillStat<Sheet.dodge, RetType>(skillStat);
			case Misc::AttackSource::dashAttack:
				return fromSkillStat<Sheet.dashAttack, RetType>(skillStat);
			case Misc::AttackSource::dodgeCounter:
				return fromSkillStat<Sheet.dodgeCounter, RetType>(skillStat);
			case Misc::AttackSource::assist:
				return fromSkillStat<Sheet.assist, RetType>(skillStat);
			case Misc::AttackSource::quickAssist:
				return fromSkillStat<Sheet.quickAssist, RetType>(skillStat);
			case Misc::AttackSource::defensiveAssist:
				return fromSkillStat<Sheet.defensiveAssist, RetType>(skillStat);
			case Misc::AttackSource::evasiveAssist:
				return fromSkillStat<Sheet.evasiveAssist, RetType>(skillStat);
			case Misc::AttackSource::assistFollowup:
				return fromSkillStat<Sheet.assistFollowup, RetType>(skillStat);
			case Misc::AttackSource::special:
				return fromSkillStat<Sheet.special, RetType>(skillStat);
			case Misc::AttackSource::exspecial:
				return fromSkillStat<Sheet.exspecial, RetType>(skillStat);
			case Misc::AttackSource::chain:
				return fromSkillStat<Sheet.chain, RetType>(skillStat);
			case Misc::AttackSource::ultimate:
				return fromSkillStat<Sheet.ultimate, RetType>(skillStat);
		}
		std::unreachable();
	}

	template<SheetLike T, class RetType = Formula::FloatNode>
	[[nodiscard]] constexpr RetType fromAttackSource(T &sheet, Misc::AttackSource attackSource, Misc::SkillStat skillStat) {
		switch (attackSource) {
			case Misc::AttackSource::basic:
				return fromSkillStat(sheet.basic, skillStat);
			case Misc::AttackSource::dodge:
				return fromSkillStat(sheet.dodge, skillStat);
			case Misc::AttackSource::dashAttack:
				return fromSkillStat(sheet.dashAttack, skillStat);
			case Misc::AttackSource::dodgeCounter:
				return fromSkillStat(sheet.dodgeCounter, skillStat);
			case Misc::AttackSource::assist:
				return fromSkillStat(sheet.assist, skillStat);
			case Misc::AttackSource::quickAssist:
				return fromSkillStat(sheet.quickAssist, skillStat);
			case Misc::AttackSource::defensiveAssist:
				return fromSkillStat(sheet.defensiveAssist, skillStat);
			case Misc::AttackSource::evasiveAssist:
				return fromSkillStat(sheet.evasiveAssist, skillStat);
			case Misc::AttackSource::assistFollowup:
				return fromSkillStat(sheet.assistFollowup, skillStat);
			case Misc::AttackSource::special:
				return fromSkillStat(sheet.special, skillStat);
			case Misc::AttackSource::exspecial:
				return fromSkillStat(sheet.exspecial, skillStat);
			case Misc::AttackSource::chain:
				return fromSkillStat(sheet.chain, skillStat);
			case Misc::AttackSource::ultimate:
				return fromSkillStat(sheet.ultimate, skillStat);
		}
		std::unreachable();
	}
	[[nodiscard]] constexpr auto &&fromAttackSource(auto &&sheet, Misc::AttackSource attackSource) {
		switch (attackSource) {
			case Misc::AttackSource::basic:
				return sheet.basic;
			case Misc::AttackSource::dodge:
				return sheet.dodge;
			case Misc::AttackSource::dashAttack:
				return sheet.dashAttack;
			case Misc::AttackSource::dodgeCounter:
				return sheet.dodgeCounter;
			case Misc::AttackSource::assist:
				return sheet.assist;
			case Misc::AttackSource::quickAssist:
				return sheet.quickAssist;
			case Misc::AttackSource::defensiveAssist:
				return sheet.defensiveAssist;
			case Misc::AttackSource::evasiveAssist:
				return sheet.evasiveAssist;
			case Misc::AttackSource::assistFollowup:
				return sheet.assistFollowup;
			case Misc::AttackSource::special:
				return sheet.special;
			case Misc::AttackSource::exspecial:
				return sheet.exspecial;
			case Misc::AttackSource::chain:
				return sheet.chain;
			case Misc::AttackSource::ultimate:
				return sheet.ultimate;
		}
		std::unreachable();
	}

	// template<auto Sheet, Misc::Element element>
	// [[nodiscard]] constexpr auto fromElement() {
	// 	if constexpr (element == Misc::Element::pyro) return Sheet.pyro;
	// 	if constexpr (element == Misc::Element::hydro) return Sheet.hydro;
	// 	if constexpr (element == Misc::Element::cryo) return Sheet.cryo;
	// 	if constexpr (element == Misc::Element::electro) return Sheet.electro;
	// 	if constexpr (element == Misc::Element::dendro) return Sheet.dendro;
	// 	if constexpr (element == Misc::Element::anemo) return Sheet.anemo;
	// 	if constexpr (element == Misc::Element::geo) return Sheet.geo;
	// 	if constexpr (element == Misc::Element::physical) return Sheet.physical;
	// }

	template<SheetLike T>
	[[nodiscard]] constexpr auto &fromAttribute(T &sheet, Misc::Attribute attribute) {
		switch (attribute) {
			case Misc::Attribute::physical:
				return sheet.physical;
			case Misc::Attribute::fire:
				return sheet.fire;
			case Misc::Attribute::ice:
				return sheet.ice;
			case Misc::Attribute::electric:
				return sheet.electric;
			case Misc::Attribute::ether:
				return sheet.ether;
		}
		std::unreachable();
	}

	template<class T, class RetType = Formula::FloatNode>
	[[nodiscard]] constexpr const RetType &fromDamageAttribute(T &sheet, Misc::DamageAttribute attribute, Misc::SkillStat skillStat) {
		switch (attribute) {
			case Misc::DamageAttribute::physical:
				return fromSkillStat(sheet.physical, skillStat);
			case Misc::DamageAttribute::fire:
				return fromSkillStat(sheet.fire, skillStat);
			case Misc::DamageAttribute::ice:
				return fromSkillStat(sheet.ice, skillStat);
			case Misc::DamageAttribute::electric:
				return fromSkillStat(sheet.electric, skillStat);
			case Misc::DamageAttribute::ether:
				return fromSkillStat(sheet.ether, skillStat);
			case Misc::DamageAttribute::all:
				return fromSkillStat(sheet.all, skillStat);
		}
		std::unreachable();
	}

	[[nodiscard]] constexpr auto &&fromDamageAttribute(auto &&sheet, Misc::DamageAttribute attribute) {
		switch (attribute) {
			case Misc::DamageAttribute::physical:
				return sheet.physical;
			case Misc::DamageAttribute::fire:
				return sheet.fire;
			case Misc::DamageAttribute::ice:
				return sheet.ice;
			case Misc::DamageAttribute::electric:
				return sheet.electric;
			case Misc::DamageAttribute::ether:
				return sheet.ether;
			case Misc::DamageAttribute::all:
				return sheet.all;
		}
		std::unreachable();
	}

	template<auto Sheet, class RetType = Formula::FloatNode>
	[[nodiscard]] constexpr RetType fromDamageAttribute(Misc::DamageAttribute attribute, Misc::SkillStat skillStat) {
		switch (attribute) {
			case Misc::DamageAttribute::physical:
				return fromSkillStat<Sheet.physical, RetType>(skillStat);
			case Misc::DamageAttribute::fire:
				return fromSkillStat<Sheet.fire, RetType>(skillStat);
			case Misc::DamageAttribute::ice:
				return fromSkillStat<Sheet.ice, RetType>(skillStat);
			case Misc::DamageAttribute::electric:
				return fromSkillStat<Sheet.electric, RetType>(skillStat);
			case Misc::DamageAttribute::ether:
				return fromSkillStat<Sheet.ether, RetType>(skillStat);
			case Misc::DamageAttribute::all:
				return fromSkillStat<Sheet.all, RetType>(skillStat);
		}
		std::unreachable();
	}

	// template<auto Skill, Misc::SkillStat skillStat>
	// [[nodiscard]] constexpr auto fromSkillStat() {
	// 	if constexpr (skillStat == Misc::SkillStat::DMG) return Skill.DMG;
	// 	if constexpr (skillStat == Misc::SkillStat::additiveDMG) return Skill.additiveDMG;
	// 	if constexpr (skillStat == Misc::SkillStat::multiplicativeDMG) return Skill.multiplicativeDMG;
	// 	if constexpr (skillStat == Misc::SkillStat::elevation) return Skill.elevation;
	// 	if constexpr (skillStat == Misc::SkillStat::critRate) return Skill.critRate;
	// 	if constexpr (skillStat == Misc::SkillStat::critDMG) return Skill.critDMG;
	// }

	template<class T, class RetType = Formula::FloatNode>
	[[nodiscard]] constexpr const RetType &fromEnemyResAttribute(T &sheet, Misc::Attribute attribute) {
		switch (attribute) {
			case Misc::Attribute::physical:
				return sheet.physical;
			case Misc::Attribute::fire:
				return sheet.fire;
			case Misc::Attribute::ice:
				return sheet.ice;
			case Misc::Attribute::electric:
				return sheet.electric;
			case Misc::Attribute::ether:
				return sheet.ether;
		}
		std::unreachable();
	}

	template<auto Sheet, class RetType = Formula::FloatNode>
	[[nodiscard]] constexpr RetType fromEnemyResAttribute(Misc::Attribute attribute) {
		switch (attribute) {
			case Misc::Attribute::physical:
				return Sheet.physical;
			case Misc::Attribute::fire:
				return Sheet.fire;
			case Misc::Attribute::ice:
				return Sheet.ice;
			case Misc::Attribute::electric:
				return Sheet.electric;
			case Misc::Attribute::ether:
				return Sheet.ether;
		}
		std::unreachable();
	}
	// template<auto Sheet>
	// [[nodiscard]] constexpr auto evalEnemyResElement(Misc::Element element, const Formula::Context &ctx) {
	// 	switch (element) {
	// 		case Misc::Element::pyro:
	// 			return Sheet.pyro.eval(ctx);
	// 		case Misc::Element::hydro:
	// 			return Sheet.hydro.eval(ctx);
	// 		case Misc::Element::cryo:
	// 			return Sheet.cryo.eval(ctx);
	// 		case Misc::Element::electro:
	// 			return Sheet.electro.eval(ctx);
	// 		case Misc::Element::dendro:
	// 			return Sheet.dendro.eval(ctx);
	// 		case Misc::Element::anemo:
	// 			return Sheet.anemo.eval(ctx);
	// 		case Misc::Element::geo:
	// 			return Sheet.geo.eval(ctx);
	// 		case Misc::Element::physical:
	// 			return Sheet.physical.eval(ctx);
	// 	}
	// 	std::unreachable();
	// }
	// template<auto Sheet>
	// [[nodiscard]] constexpr auto compileEnemyResElement(Misc::Element element, const Formula::Context &ctx) {
	// 	switch (element) {
	// 		case Misc::Element::pyro:
	// 			return Sheet.pyro.compile(ctx);
	// 		case Misc::Element::hydro:
	// 			return Sheet.hydro.compile(ctx);
	// 		case Misc::Element::cryo:
	// 			return Sheet.cryo.compile(ctx);
	// 		case Misc::Element::electro:
	// 			return Sheet.electro.compile(ctx);
	// 		case Misc::Element::dendro:
	// 			return Sheet.dendro.compile(ctx);
	// 		case Misc::Element::anemo:
	// 			return Sheet.anemo.compile(ctx);
	// 		case Misc::Element::geo:
	// 			return Sheet.geo.compile(ctx);
	// 		case Misc::Element::physical:
	// 			return Sheet.physical.compile(ctx);
	// 	}
	// 	std::unreachable();
	// }
}// namespace Stats
