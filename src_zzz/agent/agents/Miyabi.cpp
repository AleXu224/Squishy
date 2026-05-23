#include "Miyabi.hpp"

#include "agent/setup.hpp"

const Agent::Data Agent::Datas::miyabi{
	.key{1091},
	.factionId = 6,
	.goodKey{"Miyabi"},
	.name{"Miyabi"},
	.baseStats{
		.baseHp = 617,
		.baseAtk = 127,
		.baseDef = 49,
		.baseImpact = 86,
		.baseAm = 116,
		.baseAp = 148,
		.coreStat1 = Stat::atk,
		.coreStat2 = Stat::ap,
		.rarity = 5,
		.attribute = Misc::Attribute::ice,
		.specialty = Misc::Specialty::anomaly,
		.hpGrowth = 83.7238,
		.atkGrowth = 7.6728,
		.defGrowth = 6.6203,
		.hpUpgrade = {0.000, 423.000, 847.000, 1270.000, 1694.000, 2117.000},
		.atkUpgrade = {0.000, 45.000, 90.000, 136.000, 181.000, 226.000},
		.defUpgrade = {0.000, 33.000, 67.000, 100.000, 134.000, 167.000},
		.coreStat1Upgrade = {0.000, 25.000, 25.000, 50.000, 50.000, 75.000},
		.coreStat2Upgrade = {30.000, 30.000, 60.000, 60.000, 90.000, 90.000},
	},
	.setup = []() -> Data::Setup {
		auto multiplier1 = CoreMultiplier(true, {7.5f, 8.75f, 10.f, 11.25f, 12.5f, 13.75f, 15.f});

		auto coreCond = SpecialtyCountOthers{.specialty = Misc::Specialty::support} >= 1 || FactionCountOthers{.factionId = 6} >= 1;
		auto coreBasicBuff = Requires{
			.requirement = coreCond,
			.ret = Constant{.value = 0.6f},
		};

		auto coreDisorderCond = IsActive("miyabiCoreDisorderCond");
		auto coreDisorderBuff = Requires{
			.requirement = coreDisorderCond,
			.ret = Constant{.value = -0.3f},
		};

		auto ultimateCond = IsActive("miyabiUltActive");
		auto ultimateBuff = Requires{
			.requirement = ultimateCond,
			.ret = Constant{.value = 0.3f},
		};

		auto m1Stacks = GetFloat("miyabiM1FallenFrostConsumed");
		auto m1Buff = Requires{
			.requirement = Requirement::mindscape1,
			.ret = m1Stacks * 0.06f,
		};

		auto m2Buff = Requires{
			.requirement = Requirement::mindscape2,
			.ret = Constant{.value = 0.3f},
		};
		auto m2CrBuff = Requires{
			.requirement = Requirement::mindscape2,
			.ret = Constant{.value = 0.15f},
		};

		auto m4Buff = Requires{
			.requirement = Requirement::mindscape4,
			.ret = Constant{.value = 0.3f},
		};

		auto m6Buff = Requires{
			.requirement = Requirement::mindscape6,
			.ret = Constant{.value = 0.3f},
		};

		return Data::Setup{
			.mods{
				.combat{
					.cr = m2CrBuff,
					.ice{
						.DMG = ultimateBuff,
					},
					.dodgeCounter{
						.DMG = m2Buff,
					},
				},
			},
			.opts{
				.ultimate{
					Option::Boolean{
						.key = "miyabiUltActive",
						.name = "After using Ultimate",
						.mods{
							.combat{
								.ice{
									.DMG = ultimateBuff,
								},
							},
						},
					},
				},
				.core{
					Option::Boolean{
						.key = "miyabiCoreDisorderCond",
						.name = "After a squad member triggers Disorder",
						.nodes{
							Node::Info{
								.name = "Shimotsuki Stance Ice RES ignore",
								.type = Utils::EntryType::multiplier,
								.formula = coreDisorderBuff,
							},
						},
					},
				},
				.mindscape1{
					Option::ValueSlider{
						.key = "miyabiM1FallenFrostConsumed",
						.name = "Fallen Frost consumed",
						.values{0.f, 2.f, 4.f, 6.f},
						.nodes{
							Node::Info{
								.name = "Basic Attack: Shimotsuki DEF ignore",
								.type = Utils::EntryType::multiplier,
								.formula = m1Buff,
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
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 0.2690f, 0.0250f),
						.modifier{
							.DMG = m2Buff,
						},
					},
					Node::Atk{
						.name = "2nd-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 0.2960f, 0.0270f),
						.modifier{
							.DMG = m2Buff,
						},
					},
					Node::Atk{
						.name = "3rd-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 0.6280f, 0.0580f),
						.modifier{
							.DMG = m2Buff,
						},
					},
					Node::Atk{
						.name = "4th-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 0.9650f, 0.0880f),
						.modifier{
							.DMG = m2Buff,
						},
					},
					Node::Atk{
						.name = "5th-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 1.2900f, 0.1180f),
						.modifier{
							.DMG = m2Buff,
						},
					},
					Node::Daze{
						.name = "1st-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.1350f, 0.0070f)
					},
					Node::Daze{
						.name = "2nd-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.2690f, 0.0130f)
					},
					Node::Daze{
						.name = "3rd-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.4640f, 0.0220f)
					},
					Node::Daze{
						.name = "4th-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.8210f, 0.0380f)
					},
					Node::Daze{
						.name = "5th-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 1.3100f, 0.0600f)
					},
					Node::Atk{
						.name = "Charge Level One Slash DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 4.5470f, 0.4140f),
						.modifier{
							.DMG = coreBasicBuff + m6Buff,
							.enemy{
								.DEFIgnored = m1Buff,
								.resistance{
									.ice = coreDisorderBuff,
								},
							},
						},
					},
					Node::Atk{
						.name = "Charge Level Two Slash DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 8.5810f, 0.7810f),
						.modifier{
							.DMG = coreBasicBuff + m6Buff,
							.enemy{
								.DEFIgnored = m1Buff,
								.resistance{
									.ice = coreDisorderBuff,
								},
							},
						},
					},
					Node::Atk{
						.name = "Charge Level Three Slash DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 21.4110f, 1.9470f),
						.modifier{
							.DMG = coreBasicBuff + m6Buff,
							.enemy{
								.DEFIgnored = m1Buff,
								.resistance{
									.ice = coreDisorderBuff,
								},
							},
						},
					},
					Node::Daze{
						.name = "Charge Level One Slash Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.4400f, 0.0200f)
					},
					Node::Daze{
						.name = "Charge Level Two Slash Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.6240f, 0.0290f)
					},
					Node::Daze{
						.name = "Charge Level Three Slash Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 3.7780f, 0.1720f)
					},
				},
				.dashAttack{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::dodge,
						.formula = Multiplier(combat.atk, LevelableSkill::dodge, 0.2580f, 0.0240f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::dodge,
						.formula = Multiplier(combat.impact, LevelableSkill::dodge, 0.1290f, 0.0060f)
					},
				},
				.dodgeCounter{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::dodge,
						.formula = Multiplier(combat.atk, LevelableSkill::dodge, 2.4590f, 0.2240f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::dodge,
						.formula = Multiplier(combat.impact, LevelableSkill::dodge, 2.1450f, 0.0980f)
					},
				},
				.quickAssist{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.atk, LevelableSkill::assist, 1.0450f, 0.0950f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 1.0450f, 0.0480f)
					},
				},
				.defensiveAssist{
					Node::Daze{
						.name = "Light Defensive Daze",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 2.7130f, 0.1240f)
					},
					Node::Daze{
						.name = "Heavy Defensive Daze",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 3.4280f, 0.1560f)
					},
					Node::Daze{
						.name = "Chain Defensive Daze",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 1.2830f, 0.0590f)
					},
				},
				.assistFollowup{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.atk, LevelableSkill::assist, 3.3780f, 0.3080f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 2.9190f, 0.1330f)
					},
				},
				.special{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 0.3580f, 0.0330f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 0.3580f, 0.0170f)
					},
				},
				.exspecial{
					Node::Atk{
						.name = "Slash DMG",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 1.5740f, 0.1440f)
					},
					Node::Atk{
						.name = "Follow-Up DMG",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 1.9330f, 0.1760f)
					},
					Node::Daze{
						.name = "Slash Daze",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 1.2870f, 0.0590f)
					},
					Node::Daze{
						.name = "Follow-Up Daze",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 1.6200f, 0.0740f)
					},
					Node::Info{
						.name = "Slash Energy Cost",
						.type = Utils::EntryType::energy,
						.formula = Multiplier(Utils::EntryType::energy, LevelableSkill::special, 40.f, 0.f)
					},
					Node::Info{
						.name = "Follow-Up Energy Cost",
						.type = Utils::EntryType::energy,
						.formula = Multiplier(Utils::EntryType::energy, LevelableSkill::special, 40.f, 0.f)
					},
				},
				.chain{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::chain,
						.formula = Multiplier(combat.atk, LevelableSkill::chain, 1.8840f, 0.1720f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::chain,
						.formula = Multiplier(combat.impact, LevelableSkill::chain, 0.5670f, 0.0260f)
					},
				},
				.ultimate{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::chain,
						.formula = Multiplier(combat.atk, LevelableSkill::chain, 23.8800f, 2.1710f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::chain,
						.formula = Multiplier(combat.impact, LevelableSkill::chain, 3.7040f, 0.1690f)
					},
				},
				.core{
					Node::CustomAtk{
						.name = "Frostburn - Break DMG",
						.attribute = Misc::Attribute::ice,
						.formula = combat.atk * multiplier1,
						.modifier{
							.DMG = m4Buff,
						},
					},
				},
				.mindscape2{
					Node::Info{
						.name = "Basic Attack: Kazahana DMG Increase",
						.type = Utils::EntryType::multiplier,
						.formula = m2Buff,
					},
					Node::Mods{
						.mods{
							.combat{
								.cr = m2CrBuff,
								.dodgeCounter{
									.DMG = m2Buff,
								},
							},
						},
					},
				},
				.mindscape4{
					Node::Info{
						.name = "Frostburn - Break DMG Increase",
						.type = Utils::EntryType::multiplier,
						.formula = m4Buff,
					},
				},
				.mindscape6{
					Node::Info{
						.name = "Basic Attack: Shimotsuki DMG Increase",
						.type = Utils::EntryType::multiplier,
						.formula = m6Buff,
					},
				},
			},
		};
	},
};
