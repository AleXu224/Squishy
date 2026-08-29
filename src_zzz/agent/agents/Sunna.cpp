#include "Sunna.hpp"

#include "agent/setup.hpp"

const Agent::Data Agent::Datas::sunna{
	.key{1491},
	.factionId = 13,
	.goodKey{"Sunna"},
	.name{"Sunna"},
	.baseStats{
		.baseHp = 681,
		.baseAtk = 108,
		.baseDef = 48,
		.baseImpact = 98,
		.baseAm = 96,
		.baseAp = 95,
		.coreStat1 = Stat::atk,
		.coreStat2 = Stat::atk_,
		.rarity = 5,
		.attribute = Misc::Attribute::physical,
		.specialty = Misc::Specialty::support,
		.hpGrowth = 92.4944,
		.atkGrowth = 6.3517,
		.defGrowth = 6.5524,
		.hpUpgrade = {0.000, 468.000, 936.000, 1403.000, 1871.000, 2339.000},
		.atkUpgrade = {0.000, 39.000, 77.000, 116.000, 154.000, 193.000},
		.defUpgrade = {0.000, 33.000, 66.000, 99.000, 133.000, 166.000},
		.coreStat1Upgrade = {0.000, 25.000, 25.000, 50.000, 50.000, 75.000},
		.coreStat2Upgrade = {0.070, 0.070, 0.140, 0.140, 0.210, 0.210},
	},
	.setup = []() -> Data::Setup {
		auto multiplier1 = CoreMultiplier(false, {525.f, 615.f, 705.f, 795.f, 885.f, 975.f, 1050.f});
		auto multiplier2 = CoreMultiplier(false, {1750.f, 2050.f, 2350.f, 2650.f, 2950.f, 3250.f, 3500.f});
		auto multiplier3 = CoreMultiplier(true, {1.5f, 1.75f, 2.f, 2.25f, 2.5f, 2.75f, 3.f});
		auto multiplier4 = CoreMultiplier(true, {2.4f, 2.8f, 3.2f, 3.6f, 4.f, 4.4f, 4.8f});
		auto multiplier5 = CoreMultiplier(true, {0.8f, 0.9f, 1.f, 1.2f, 1.3f, 1.4f, 1.5f});

		auto angelicChordCond = IsActive("angelicChordCond");
		auto etherVeilCond = IsActive("etherVeilCond") && angelicChordCond;
		auto etherVeilAtk = Requires{
			.requirement = etherVeilCond,
			.ret = ConstantFlat{.value = 50.f},
		};

		auto angelicChordAtk = Requires{
			.requirement = angelicChordCond,
			.ret = Min{.val1 = initial.atk * 0.3f, .val2 = multiplier1},
		};

		auto additionalCond = SpecialtyCountOthers{.specialty = Misc::Specialty::attack} >= 1 || FactionCountOthers{.factionId = 13} >= 1;
		auto additionalEtherVeilStunMulti = Requires{
			.requirement = additionalCond && etherVeilCond,
			.ret = Constant{.value = 0.3f},
		};

		auto m1Stacks = GetFloat("m1Stacks");
		auto m1Buff = Requires{
			.requirement = Requirement::mindscape1,
			.ret = m1Stacks * 0.07f,
		};

		auto m2Cond = IsActive("m2Cond");
		auto m2Buff = Requires{
			.requirement = (m2Cond || etherVeilCond) && Requirement::mindscape2,
			.ret = Constant{.value = 0.1f},
		};

		auto m4Cond = IsActive("m4UltimateActive");
		auto m4Buff = Requires{
			.requirement = m4Cond && Requirement::mindscape4,
			.ret = Constant{.value = 0.18f},
		};

		auto m6Cr = Requires{
			.requirement = Requirement::mindscape6 && angelicChordCond,
			.ret = Constant{.value = 1.f},
		};
		auto m6Cd = Requires{
			.requirement = Requirement::mindscape6 && angelicChordCond,
			.ret = Min{.val1 = 0.0003f * initial.atk, .val2 = Constant{.value = 1.05f}},
		};

		// FIXME: no Cat's Gaze implemented at all because it is quite annoying to implement and unoptimizable except m6

		return Data::Setup{
			.mods{
				.combat{
					.cr = m6Cr,
					.cd = m6Cd,
				},
				.teamCombat{
					.atk = etherVeilAtk + angelicChordAtk,
					.atk_ = m2Buff,
					.all{
						.DMG = m4Buff,
					},
				},
				.enemy{
					.stunMod = additionalEtherVeilStunMulti,
					.DEFReduction = m1Buff,
				},
			},
			.opts{
				.exspecial{
					Option::Boolean{
						.key = "angelicChordCond",
						.name = "EX Special Active",
						.teamBuff = true,
						.mods{
							.combat{
								.cr = m6Cr,
								.cd = m6Cd,
							},
							.teamCombat{
								.atk = angelicChordAtk,
							},
						},
					},
					Option::Boolean{
						.key = "etherVeilCond",
						.name = "Ether Veil Active",
						.teamBuff = true,
						.displayCondition = angelicChordCond,
						.mods{
							.teamCombat{
								.atk = etherVeilAtk,
							},
							.enemy{
								.stunMod = additionalEtherVeilStunMulti,
							},
						},
					},
				},
				.mindscape1{
					Option::ValueSlider{
						.key = "m1Stacks",
						.name = "Cat's Gaze triggers",
						.teamBuff = true,
						.values{0, 1, 2, 3},
						.mods{
							.enemy{
								.DEFReduction = m1Buff,
							},
						},
					},
				},
				.mindscape2{
					Option::Boolean{
						.key = "m2Cond",
						.name = "Within any Ether Veil",
						.teamBuff = true,
						.mods{
							.teamCombat{
								.atk_ = m2Buff,
							},
						},
					},
				},
				.mindscape4{
					Option::Boolean{
						.key = "m4UltimateActive",
						.name = "After Sunna uses her Ultimate",
						.teamBuff = true,
						.mods{
							.teamCombat{
								.all{
									.DMG = m4Buff,
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
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 0.4340f, 0.0400f)
					},
					Node::Atk{
						.name = "2nd-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 1.7590f, 0.1600f)
					},
					Node::Atk{
						.name = "3rd-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 1.8120f, 0.1650f)
					},
					Node::Atk{
						.name = "4th-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 4.0530f, 0.3690f)
					},
					Node::Daze{
						.name = "1st-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.2480f, 0.0120f)
					},
					Node::Daze{
						.name = "2nd-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 1.0910f, 0.0500f)
					},
					Node::Daze{
						.name = "3rd-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 1.2360f, 0.0570f)
					},
					Node::Daze{
						.name = "4th-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 2.5170f, 0.1150f)
					},
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 2.0800f, 0.1900f)
					},
				},
				.dashAttack{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::dashAttack,
						.formula = Multiplier(combat.atk, LevelableSkill::dodge, 0.9970f, 0.0910f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::dashAttack,
						.formula = Multiplier(combat.impact, LevelableSkill::dodge, 0.4990f, 0.0230f)
					},
				},
				.dodgeCounter{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::dodgeCounter,
						.formula = Multiplier(combat.atk, LevelableSkill::dodge, 3.1880f, 0.2900f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::dodgeCounter,
						.formula = Multiplier(combat.impact, LevelableSkill::dodge, 2.7530f, 0.1260f)
					},
				},
				.quickAssist{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::quickAssist,
						.formula = Multiplier(combat.atk, LevelableSkill::assist, 0.7270f, 0.0670f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::quickAssist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 0.7270f, 0.0340f)
					},
				},
				.defensiveAssist{
					Node::Daze{
						.name = "Light Defensive Daze",
						.source = Misc::AttackSource::defensiveAssist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 3.2070f, 0.1460f)
					},
					Node::Daze{
						.name = "Heavy Defensive Daze",
						.source = Misc::AttackSource::defensiveAssist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 4.0520f, 0.1850f)
					},
					Node::Daze{
						.name = "Chain Defensive Daze",
						.source = Misc::AttackSource::defensiveAssist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 1.9720f, 0.0900f)
					},
				},
				.assistFollowup{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::assistFollowup,
						.formula = Multiplier(combat.atk, LevelableSkill::assist, 5.3570f, 0.4870f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::assistFollowup,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 4.7170f, 0.2150f)
					},
				},
				.special{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 0.5640f, 0.0520f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 0.5640f, 0.0260f)
					},
				},
				.exspecial{
					Node::Atk{
						.name = "Bubblegum Barrage DMG",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 7.9410f, 0.7220f)
					},
					Node::Daze{
						.name = "Bubblegum Barrage Daze",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 4.9460f, 0.2250f)
					},
					Node::Atk{
						.name = "Bubblegum Barrage DMG when quickly chaining into Special Photography Technique",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 7.9410f, 0.7220f)
					},
					Node::Daze{
						.name = "Bubblegum Barrage Daze when quickly chaining into Special Photography Technique",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 4.9460f, 0.2250f)
					},
					Node::Info{
						.name = "Bubblegum Barrage Energy Cost",
						.type = Utils::EntryType::energy,
						.formula = Multiplier(Utils::EntryType::energy, LevelableSkill::special, 70.f, 0.f)
					},
					Node::Atk{
						.name = "Special Photography Technique DMG",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 8.2810f, 0.7530f)
					},
					Node::Daze{
						.name = "Special Photography Technique Daze",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 6.0230f, 0.2740f)
					},
					Node::Atk{
						.name = "Special Photography Technique DMG (Coordinated)",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 8.2810f, 0.7530f)
					},
					Node::Daze{
						.name = "Special Photography Technique Daze (Coordinated)",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 6.0230f, 0.2740f)
					},
				},
				.chain{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::chain,
						.formula = Multiplier(combat.atk, LevelableSkill::chain, 8.1130f, 0.7380f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::chain,
						.formula = Multiplier(combat.impact, LevelableSkill::chain, 2.9260f, 0.1330f)
					},
				},
				.ultimate{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::ultimate,
						.formula = Multiplier(combat.atk, LevelableSkill::chain, 20.9090f, 1.9010f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::ultimate,
						.formula = Multiplier(combat.impact, LevelableSkill::chain, 3.7280f, 0.1700f)
					},
					Node::Info{
						.name = "HP Recovery",
						.type = Utils::EntryType::points,
						.formula = Multiplier(Utils::EntryType::points, LevelableSkill::chain, -50.f, 250.f) + ConstantFlat{.value = 250.f},
					},
				},
			},
		};
	},
};
