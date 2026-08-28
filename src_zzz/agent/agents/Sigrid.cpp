#include "Sigrid.hpp"

#include "agent/setup.hpp"

const Agent::Data Agent::Datas::sigrid{
	.key{1591},
	.factionId = 16,
	.goodKey{"Sigrid"},
	.name{"Sigrid"},
	.baseStats{
		.baseHp = 626,
		.baseAtk = 135,
		.baseDef = 49,
		.baseImpact = 93,
		.baseAm = 92,
		.baseAp = 90,
		.coreStat1 = Stat::atk,
		.coreStat2 = Stat::cr,
		.rarity = 5,
		.attribute = Misc::Attribute::ice,
		.specialty = Misc::Specialty::attack,
		.hpGrowth = 84.9779,
		.atkGrowth = 8.2578,
		.defGrowth = 6.6203,
		.hpUpgrade = {0.000, 430.000, 859.000, 1289.000, 1719.000, 2149.000},
		.atkUpgrade = {0.000, 48.000, 96.000, 145.000, 193.000, 241.000},
		.defUpgrade = {0.000, 33.000, 67.000, 100.000, 134.000, 167.000},
		.coreStat1Upgrade = {0.000, 25.000, 25.000, 50.000, 50.000, 75.000},
		.coreStat2Upgrade = {0.048, 0.048, 0.096, 0.096, 0.144, 0.144},
	},
	.setup = []() -> Data::Setup {
		auto multiplier1 = CoreMultiplier(true, {0.33f, 0.39f, 0.44f, 0.5f, 0.55f, 0.61f, 0.66f});

		auto aerialPatrolSpear = IsActive("aerialPatrolSpearCond");
		auto aerialPatrolSpearBuff = Requires{
			.requirement = aerialPatrolSpear,
			.ret = multiplier1,
		};
		auto aerialPatrolSpearStunMultiplier = Requires{
			.requirement = aerialPatrolSpear && Requirement::enemyStunned,
			.ret = Constant{.value = 0.2f},
		};

		auto additionalCond = SpecialtyCountOthers{.specialty = Misc::Specialty::support} >= 1 || SpecialtyCountOthers{.specialty = Misc::Specialty::stun} >= 1;
		auto additionalAtk = Requires{
			.requirement = additionalCond,
			.ret = Min{.val1 = ConstantFlat{.value = 120.f} + agent.level * 12, .val2 = ConstantFlat{.value = 840.f}},
		};

		auto contaminationCond = IsActive("contaminationCond");
		auto contaminationBuff = Requires{
			.requirement = contaminationCond && additionalCond,
			.ret = Constant{.value = 0.15f},
		};

		auto m1Buff = Requires{
			.requirement = Requirement::mindscape1,
			.ret = Constant{.value = 0.25f},
		};

		auto m2Buff = Requires{
			.requirement = Requirement::mindscape2,
			.ret = Constant{.value = 0.24f},
		};

		auto m4Buff = Requires{
			.requirement = aerialPatrolSpear && Requirement::mindscape4,
			.ret = Constant{.value = 0.18f},
		};

		return Data::Setup{
			.mods{
				.combat{
					.atk = additionalAtk,
					.atk_ = m1Buff,
					.cr = aerialPatrolSpearBuff,
					.all{
						.DMG = contaminationBuff + m4Buff,
					},
				},
				.enemy{
					.stunMod = aerialPatrolSpearStunMultiplier,
				},
			},
			.opts{
				.core{
					Option::Boolean{
						.key = "aerialPatrolSpearCond",
						.name = "Aerial Patrol Spear active",
						.mods{
							.combat{
								.cr = aerialPatrolSpearBuff,
							},
							.enemy{
								.stunMod = aerialPatrolSpearStunMultiplier,
							},
						},
					},
					Option::Boolean{
						.key = "contaminationCond",
						.name = "Enemy in Contamination state",
						.mods{
							.combat{
								.all{
									.DMG = contaminationBuff + m4Buff,
								},
							},
						},
					},
				},
			},
			.nodes{
				.basic{
					Node::Atk{
						.name = "1st-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 0.7700f, 0.0700f)
					},
					Node::Atk{
						.name = "2nd-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 1.4380f, 0.1310f)
					},
					Node::Atk{
						.name = "3rd-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 1.0140f, 0.0930f)
					},
					Node::Atk{
						.name = "4th-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 2.2100f, 0.2010f),
						.modifier{
							.penRatio = m2Buff,
						},
					},
					Node::Daze{
						.name = "1st-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.4540f, 0.0210f)
					},
					Node::Daze{
						.name = "2nd-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.8870f, 0.0410f)
					},
					Node::Daze{
						.name = "3rd-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.5580f, 0.0260f)
					},
					Node::Daze{
						.name = "4th-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 1.3850f, 0.0630f)
					},
					Node::Atk{
						.name = "Converging Spear 1st Stage DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 3.5830f, 0.3260f),
						.modifier{
							.penRatio = m2Buff,
						},
					},
					Node::Daze{
						.name = "Converging Spear 1st Stage Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.8630f, 0.0400f)
					},
					Node::Atk{
						.name = "Converging Spear 2nd Stage DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 6.0910f, 0.5540f),
						.modifier{
							.penRatio = m2Buff,
						},
					},
					Node::Daze{
						.name = "Converging Spear 2nd Stage Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 1.4310f, 0.0660f)
					},
					Node::Atk{
						.name = "Converging Spear 3rd Stage DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 8.1460f, 0.7410f),
						.modifier{
							.penRatio = m2Buff,
						},
					},
					Node::Daze{
						.name = "Converging Spear 3rd Stage Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 1.9620f, 0.0900f)
					},
				},
				.dashAttack{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::dodge,
						.formula = Multiplier(combat.atk, LevelableSkill::dodge, 1.1900f, 0.1090f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::dodge,
						.formula = Multiplier(combat.impact, LevelableSkill::dodge, 0.4680f, 0.0220f)
					},
				},
				.dodgeCounter{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::dodgeCounter,
						.formula = Multiplier(combat.atk, LevelableSkill::dodge, 2.4310f, 0.2210f),
						.modifier{
							.penRatio = m2Buff,
						},
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::dodgeCounter,
						.formula = Multiplier(combat.impact, LevelableSkill::dodge, 1.7230f, 0.0790f)
					},
				},
				.quickAssist{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.atk, LevelableSkill::assist, 0.3970f, 0.0370f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 0.3120f, 0.0150f)
					},
				},
				.defensiveAssist{
					Node::Daze{
						.name = "Light Defensive Daze",
						.source = Misc::AttackSource::assistFollowup,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 2.7130f, 0.1240f)
					},
					Node::Daze{
						.name = "Heavy Defensive Daze",
						.source = Misc::AttackSource::assistFollowup,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 3.4280f, 0.1560f)
					},
					Node::Daze{
						.name = "Chain Defensive Daze",
						.source = Misc::AttackSource::assistFollowup,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 1.6680f, 0.0760f)
					},
				},
				.assistFollowup{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::assistFollowup,
						.formula = Multiplier(combat.atk, LevelableSkill::assist, 5.1490f, 0.4690f),
						.modifier{
							.penRatio = m2Buff,
						},
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::assistFollowup,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 3.5380f, 0.1610f)
					},
				},
				.special{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 0.5370f, 0.0490f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 0.4220f, 0.0200f)
					},
				},
				.exspecial{
					Node::Atk{
						.name = "Scattered Jade DMG",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 4.3880f, 0.3990f),
						.modifier{
							.penRatio = m2Buff,
						},
					},
					Node::Daze{
						.name = "Scattered Jade Daze",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 2.7430f, 0.1250f)
					},
					Node::Info{
						.name = "Scattered Jade Energy Cost",
						.type = Utils::EntryType::energy,
						.formula = Multiplier(Utils::EntryType::energy, LevelableSkill::special, 60.f, 0.f)
					},
					Node::Atk{
						.name = "Shattered Jade DMG",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 10.4780f, 0.9530f),
						.modifier{
							.penRatio = m2Buff,
						},
					},
					Node::Daze{
						.name = "Shattered Jade Daze",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 3.6260f, 0.1650f)
					},
					Node::Info{
						.name = "Shattered Jade Energy Cost",
						.type = Utils::EntryType::energy,
						.formula = Multiplier(Utils::EntryType::energy, LevelableSkill::special, 50.f, 0.f)
					},
				},
				.chain{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::chain,
						.formula = Multiplier(combat.atk, LevelableSkill::chain, 9.4370f, 0.8580f),
						.modifier{
							.penRatio = m2Buff,
						},
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::chain,
						.formula = Multiplier(combat.impact, LevelableSkill::chain, 1.3760f, 0.0630f)
					},
				},
				.ultimate{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::ultimate,
						.formula = Multiplier(combat.atk, LevelableSkill::chain, 21.8980f, 1.9910f),
						.modifier{
							.penRatio = m2Buff,
						},
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::ultimate,
						.formula = Multiplier(combat.impact, LevelableSkill::chain, 2.6580f, 0.1210f),
					},
				},
				.core{
					Node::Mods{
						.mods{
							.combat{
								.atk = additionalAtk,
							},
						},
					},
				},
				.mindscape1{
					Node::Atk{
						.name = "Additional Attack",
						.attribute = Misc::Attribute::ice,
						.source = Misc::AttackSource::basic,
						.formula = Requires{.requirement = Requirement::mindscape1, .ret = combat.atk * 1.f},
					},
					Node::Mods{
						.mods{
							.combat{
								.atk_ = m1Buff,
							},
						},
					},
				},
				.mindscape2{
					Node::Info{
						.name = "Unbridled Spear PEN Ratio%",
						.type = Utils::EntryType::multiplier,
						.formula = m2Buff,
					},
					Node::Info{
						.name = "Basic Attack: Converging Spear PEN Ratio%",
						.type = Utils::EntryType::multiplier,
						.formula = m2Buff,
					},
				},
				.mindscape6{
					Node::Atk{
						.name = "Basic Attack: Converging Spear 1st Stage Additional Attack",
						.attribute = Misc::Attribute::ice,
						.source = Misc::AttackSource::basic,
						.formula = Requires{.requirement = Requirement::mindscape6, .ret = combat.atk * 0.8f},
					},
					Node::Atk{
						.name = "Basic Attack: Converging Spear 2nd Stage Additional Attack",
						.attribute = Misc::Attribute::ice,
						.source = Misc::AttackSource::basic,
						.formula = Requires{.requirement = Requirement::mindscape6, .ret = combat.atk * 0.9f},
					},
					Node::Atk{
						.name = "Basic Attack: Converging Spear 3rd Stage Additional Attack",
						.attribute = Misc::Attribute::ice,
						.source = Misc::AttackSource::basic,
						.formula = Requires{.requirement = Requirement::mindscape6, .ret = combat.atk * 1.f},
					},
				},
			},
		};
	},
};
