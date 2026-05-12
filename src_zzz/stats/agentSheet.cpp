#include "agentSheet.hpp"

#include "formula/base.hpp"
#include "formula/custom.hpp"
#include "formula/prefix.hpp"
#include "formula/requirement.hpp"
#include "formula/requires.hpp"
#include "modifiers/total/base.hpp"
#include "modifiers/total/initial.hpp"
#include "modifiers/total/total.hpp"
#include "stats/loadout.hpp"


void Stats::AgentSheet::init(Stats::State &stats) {
	using namespace Formula::Operators;
	// HP
	this->base.hp.modifiers.at(0) = Formula::Prefix{
		.prefix = "Agent Base",
		.val = Formula::Custom{
			.foldFunc = [](const Formula::Context &context, const Formula::FoldArgs &args) -> Formula::FloatNode {
				return Formula::ConstantFlat{.value = context.source.stats.base.getHpAt(context.source.stats.sheet.level, context.source.stats.sheet.promotion)};
			},
			.func = [](const Formula::Context &context) {
				return context.source.stats.base.getHpAt(context.source.stats.sheet.level, context.source.stats.sheet.promotion);
			}
		}
	};

	// ATK
	this->base.atk.modifiers.at(0) = Formula::Prefix{
		.prefix = "Agent Base",
		.val = Formula::Custom{
			.foldFunc = [](const Formula::Context &context, const Formula::FoldArgs &args) -> Formula::FloatNode {
				return Formula::ConstantFlat{.value = context.source.stats.base.getAtkAt(context.source.stats.sheet.level, context.source.stats.sheet.promotion)};
			},
			.func = [](const Formula::Context &context) {
				return context.source.stats.base.getAtkAt(context.source.stats.sheet.level, context.source.stats.sheet.promotion);
			}
		}
	};

	// DEF
	this->base.def.modifiers.at(0) = Formula::Prefix{
		.prefix = "Agent Base",
		.val = Formula::Custom{
			.foldFunc = [](const Formula::Context &context, const Formula::FoldArgs &args) -> Formula::FloatNode {
				return Formula::ConstantFlat{.value = context.source.stats.base.getDefAt(context.source.stats.sheet.level, context.source.stats.sheet.promotion)};
			},
			.func = [](const Formula::Context &context) {
				return context.source.stats.base.getDefAt(context.source.stats.sheet.level, context.source.stats.sheet.promotion);
			}
		}
	};

	// Impact
	this->base.impact.modifiers.at(0) = Formula::Prefix{
		.prefix = "Agent Base",
		.val = Formula::Custom{
			.foldFunc = [](const Formula::Context &context, const Formula::FoldArgs &args) -> Formula::FloatNode {
				return Formula::ConstantFlat{.value = context.source.stats.base.baseImpact};
			},
			.func = [](const Formula::Context &context) {
				return context.source.stats.base.baseImpact;
			}
		}
	};

	// Anomaly Proficiency
	this->base.ap.modifiers.at(0) = Formula::Prefix{
		.prefix = "Agent Base",
		.val = Formula::Custom{
			.foldFunc = [](const Formula::Context &context, const Formula::FoldArgs &args) -> Formula::FloatNode {
				return Formula::ConstantFlat{.value = context.source.stats.base.baseAp};
			},
			.func = [](const Formula::Context &context) {
				return context.source.stats.base.baseAp;
			}
		}
	};

	// Anomaly Mastery
	this->base.am.modifiers.at(0) = Formula::Prefix{
		.prefix = "Agent Base",
		.val = Formula::Custom{
			.foldFunc = [](const Formula::Context &context, const Formula::FoldArgs &args) -> Formula::FloatNode {
				return Formula::ConstantFlat{.value = context.source.stats.base.baseAm};
			},
			.func = [](const Formula::Context &context) {
				return context.source.stats.base.baseAm;
			}
		}
	};

	// Core stats
	this->base.fromStat(stats.stats.base.coreStat1).modifiers.at(1) = Formula::Prefix{
		.prefix = "Agent Base",
		.val = Formula::Custom{
			.foldFunc = [](const Formula::Context &context, const Formula::FoldArgs &args) -> Formula::FloatNode {
				return Formula::ConstantFlat{.value = context.source.stats.base.getCoreStat1At(context.source.stats.sheet.skills.core.get(context))};
			},
			.func = [](const Formula::Context &context) {
				return context.source.stats.base.getCoreStat1At(context.source.stats.sheet.skills.core.get(context));
			},
			.isPercentage = Utils::isPercentage(stats.stats.base.coreStat1)
		}
	};
	this->base.fromStat(stats.stats.base.coreStat2).modifiers.at(1) = Formula::Prefix{
		.prefix = "Agent Base",
		.val = Formula::Custom{
			.foldFunc = [](const Formula::Context &context, const Formula::FoldArgs &args) -> Formula::FloatNode {
				return Formula::ConstantFlat{.value = context.source.stats.base.getCoreStat2At(context.source.stats.sheet.skills.core.get(context))};
			},
			.func = [](const Formula::Context &context) {
				return context.source.stats.base.getCoreStat2At(context.source.stats.sheet.skills.core.get(context));
			},
			.isPercentage = Utils::isPercentage(stats.stats.base.coreStat2)
		}
	};

	std::vector<std::pair<Stat, Stat>> mulpliableStats = {
		{Stat::atk, Stat::atk_},
		{Stat::def, Stat::def_},
		{Stat::hp, Stat::hp_},
		{Stat::er, Stat::er_},
		{Stat::am, Stat::am_},
		{Stat::ap, Stat::ap_},
		{Stat::impact, Stat::impact_},
	};

	for (const auto &[baseStat, percentStat]: mulpliableStats) {
		this->initial.fromStat(baseStat).modifiers.at(0) = Modifiers::base().fromStat(baseStat) * (1.f + Modifiers::initial().fromStat(percentStat));
		this->combat.fromStat(baseStat).modifiers.at(0) = Modifiers::initial().fromStat(baseStat) * (1.f + Modifiers::combat().fromStat(percentStat));
	}

	std::vector<Stat> transferableStats = {
		Stat::cr,
		Stat::cd,
		Stat::pen,
		Stat::penRatio,
	};
	for (const auto &stat: transferableStats) {
		this->initial.fromStat(stat).modifiers.at(0) = Modifiers::base().fromStat(stat);
		this->combat.fromStat(stat).modifiers.at(0) = Modifiers::initial().fromStat(stat);
	}
	for (const auto &attribute: Misc::damageAttributes) {
		for (const auto &skillStat: Misc::skillStats) {
			this->initial.fromDamageAttribute(attribute).fromSkillStat(skillStat).modifiers.at(0) = Modifiers::base().fromDamageAttribute(attribute).fromSkillStat(skillStat);
			this->combat.fromDamageAttribute(attribute).fromSkillStat(skillStat).modifiers.at(0) = Modifiers::initial().fromDamageAttribute(attribute).fromSkillStat(skillStat);
		}
	}
	for (const auto &attackSource: Misc::attackSources) {
		for (const auto &skillStat: Misc::skillStats) {
			this->initial.fromAttackSource(attackSource).fromSkillStat(skillStat).modifiers.at(0) = Modifiers::base().fromAttackSource(attackSource).fromSkillStat(skillStat);
			this->combat.fromAttackSource(attackSource).fromSkillStat(skillStat).modifiers.at(0) = Modifiers::initial().fromAttackSource(attackSource).fromSkillStat(skillStat);
		}
	}
	for (const auto &anomaly: Misc::damageAnomalies) {
		for (const auto &skillStat: Misc::skillStats) {
			this->initial.fromDamageAnomaly(anomaly).fromSkillStat(skillStat).modifiers.at(0) = Modifiers::base().fromDamageAnomaly(anomaly).fromSkillStat(skillStat);
			this->combat.fromDamageAnomaly(anomaly).fromSkillStat(skillStat).modifiers.at(0) = Modifiers::initial().fromDamageAnomaly(anomaly).fromSkillStat(skillStat);
		}
	}

	// Constellation skills
	for (const auto &skill: levelableSkills) {
		if (skill == LevelableSkill::core) continue;
		this->skills.fromSkill(skill).modifiers.at(0) = Formula::Requires{
			.requirement = Requirement::mindscape3,
			.ret = Formula::ConstantInt{.value = 2},
		};
		this->skills.fromSkill(skill).modifiers.at(1) = Formula::Requires{
			.requirement = Requirement::mindscape5,
			.ret = Formula::ConstantInt{.value = 2},
		};
	}
}
