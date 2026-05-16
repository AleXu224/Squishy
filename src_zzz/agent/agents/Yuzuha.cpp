#include "Yuzuha.hpp"

#include "agent/setup.hpp"

const Agent::Data Agent::Datas::yuzuha{
	.key{1411},
	.factionId = 11,
	.goodKey{"Yuzuha"},
	.name{"Yuzuha"},
	.baseStats{
		.baseHp = 710,
		.baseAtk = 109,
		.baseDef = 49,
		.baseImpact = 86,
		.baseAm = 88,
		.baseAp = 93,
		.coreStat1 = Stat::atk,
		.coreStat2 = Stat::am,
		.rarity = 5,
		.attribute = Misc::Attribute::physical,
		.specialty = Misc::Specialty::support,
		.hpGrowth = 96.33,
		.atkGrowth = 6.4272,
		.defGrowth = 6.6882,
		.hpUpgrade = {0.000, 487.000, 974.000, 1461.000, 1949.000, 2436.000},
		.atkUpgrade = {0.000, 39.000, 78.000, 117.000, 156.000, 195.000},
		.defUpgrade = {0.000, 34.000, 68.000, 101.000, 135.000, 169.000},
		.coreStat1Upgrade = {0.000, 25.000, 25.000, 50.000, 50.000, 75.000},
		.coreStat2Upgrade = {12.000, 12.000, 24.000, 24.000, 36.000, 36.000},
	},
	.setup = []() -> Data::Setup {
		auto multiplier1 = CoreMultiplier(false, {600.f, 700.f, 800.f, 900.f, 1000.f, 1100.f, 1200.f});

		auto coreCond = IsActive("yuzuhaCoreActive");
		auto coreBuff = Requires{
			.requirement = coreCond,
			.ret = Min{.val1 = initial.atk * 0.4f, .val2 = multiplier1}
		};

		auto additionalBuff = Requires{
			.requirement = coreCond
						&& (SpecialtyCountOthers{.specialty = Misc::Specialty::anomaly} >= 1 || FactionCountOthers{.factionId = 11} >= 1),
			.ret = Clamp{
				.val1 = (combat.am - ConstantFlat{.value = 100.f}) * 0.002f,
				.min = 0.f,
				.max = 0.2f,
			}
		};

		auto m1Cond = IsActive("yuzuhaMindscape1Cond");
		auto m1Buff = Requires{
			.requirement = Requirement::mindscape1 && m1Cond,
			.ret = Constant{.value = -0.1},
		};
		auto m1AdditionalBuff = Requires{
			.requirement = Requirement::mindscape1 && m1Cond,
			.ret = Constant{.value = 0.3f} * additionalBuff,
		};

		auto m2Buff = Requires{
			.requirement = Requirement::mindscape2 && coreCond,
			.ret = Constant{.value = 0.15f},
		};

		auto m4Buff = Requires{
			.requirement = Requirement::mindscape4,
			.ret = Constant{.value = 0.3f},
		};

		auto m6Stacks = GetFloat("yuzuhaMindscape6Stacks");
		auto m6Buff = Requires{
			.requirement = Requirement::mindscape6,
			.ret = OriginAgent{.formula = combat.atk} * 1.05f * m6Stacks,
		};

		auto totalAdditionalBuff = additionalBuff + m1AdditionalBuff;

		return Data::Setup{
			.mods{
				.teamCombat{
					.atk = coreBuff,
					.all{
						.DMG = m2Buff,
					},
					.disorder{
						.DMG = totalAdditionalBuff,
						.additiveDMG = m6Buff,
					},
					.allAnomaly{
						.DMG = totalAdditionalBuff,
					},
				},
				.enemy{
					.resistance{
						.physical = m1Buff,
					},
				},
			},
			.opts{
				.core{
					Option::Boolean{
						.key = "yuzuhaCoreActive",
						.name = "After EX Special or Ultimate hits an enemy",
						.teamBuff = true,
						.mods{
							.teamCombat{
								.atk = coreBuff,
								.all{
									.DMG = m2Buff,
								},
								.disorder{
									.DMG = totalAdditionalBuff,
								},
								.allAnomaly{
									.DMG = totalAdditionalBuff,
								},
							},
						},
					},
				},
				.mindscape1{
					Option::Boolean{
						.key = "yuzuhaMindscape1Cond",
						.name = "Enemy is in the Sweet Scare state",
						.teamBuff = true,
						.mods{
							.enemy{
								.resistance{
									.physical = m1Buff,
								},
							},
						},
					},
				},
				.mindscape6{
					Option::ValueSlider{
						.key = "yuzuhaMindscape6Stacks",
						.name = "After a shell from Assist Follow-Up: Stuffed Hard Candy Shot hits an enemy",
						.teamBuff = true,
						.values = std::views::iota(0, 4) | std::ranges::to<std::vector<float>>(),
						.mods{
							.teamCombat{
								.disorder{
									.additiveDMG = m6Buff,
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
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 0.3770f, 0.0350f)
					},
					Node::Atk{
						.name = "2nd-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 0.5770f, 0.0530f)
					},
					Node::Atk{
						.name = "3rd-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 0.6600f, 0.0600f)
					},
					Node::Atk{
						.name = "4th-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 0.9810f, 0.0900f)
					},
					Node::Atk{
						.name = "5th-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 1.6410f, 0.1500f)
					},
					Node::Daze{
						.name = "1st-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.4400f, 0.0200f)
					},
					Node::Daze{
						.name = "2nd-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.6310f, 0.0290f)
					},
					Node::Daze{
						.name = "3rd-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.6600f, 0.0300f)
					},
					Node::Daze{
						.name = "4th-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 1.1270f, 0.0520f)
					},
					Node::Daze{
						.name = "5th-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 1.6360f, 0.0750f)
					},
					Node::Daze{
						.name = "Parry Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 1.6680f, 0.0760f)
					},
					Node::Atk{
						.name = "Hard Candy Shot DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 1.3200f, 0.1200f)
					},
					Node::Daze{
						.name = "Hard Candy Shot Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.8800f, 0.0400f)
					},
					Node::Atk{
						.name = "Sugarburst Sparkles DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 0.2750f, 0.0250f)
					},
					Node::Info{
						.name = "Sugarburst Sparkles Anomaly Buildup Rate Increase",
						.type = Utils::EntryType::multiplier,
						.formula = 0.07f + skills.basic * 0.015f
					},
					Node::Atk{
						.name = "Sugarburst Sparkles - Max DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 3.0800f, 0.2800f)
					},
					Node::Info{
						.name = "Sugarburst Sparkles - Max Anomaly Buildup Rate Increase",
						.type = Utils::EntryType::multiplier,
						.formula = 0.07f + skills.basic * 0.015f
					},
					Node::Atk{
						.name = "Tanuki Kama & Yuzuha Coordinated Attack DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 1.1000f, 0.1000f)
					},
					Node::Atk{
						.name = "Tanuki Kama Auto-Attack DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.atk, LevelableSkill::basic, 1.1000f, 0.1000f)
					},
				},
				.dashAttack{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::dodge,
						.formula = Multiplier(combat.atk, LevelableSkill::dodge, 0.6980f, 0.0640f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::dodge,
						.formula = Multiplier(combat.impact, LevelableSkill::dodge, 0.3490f, 0.0160f)
					},
				},
				.dodgeCounter{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::dodge,
						.formula = Multiplier(combat.atk, LevelableSkill::dodge, 2.7690f, 0.2520f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::dodge,
						.formula = Multiplier(combat.impact, LevelableSkill::dodge, 2.3840f, 0.1090f)
					},
				},
				.quickAssist{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.atk, LevelableSkill::assist, 1.2290f, 0.1120f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 0.6150f, 0.0280f)
					},
				},
				.defensiveAssist{
					Node::Daze{
						.name = "Light Defensive Daze",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 2.9520f, 0.1350f)
					},
					Node::Daze{
						.name = "Heavy Defensive Daze",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 3.4280f, 0.1560f)
					},
					Node::Daze{
						.name = "Chain Defensive Daze",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 1.6680f, 0.0760f)
					},
				},
				.assistFollowup{
					Node::Atk{
						.name = "We Have Cookies DMG",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.atk, LevelableSkill::assist, 2.9430f, 0.2680f),
						.modifier{
							.DMG = m4Buff,
						}
					},
					Node::Daze{
						.name = "We Have Cookies Daze",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 2.5140f, 0.1150f)
					},
					Node::Atk{
						.name = "Stuffed Hard Candy Shot DMG",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.atk, LevelableSkill::assist, 4.6360f, 0.4220f),
						.modifier{
							.DMG = m4Buff,
						}
					},
					Node::Daze{
						.name = "Stuffed Hard Candy Shot Daze",
						.source = Misc::AttackSource::assist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 4.0870f, 0.1860f)
					},
				},
				.special{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 0.5050f, 0.0460f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 0.5050f, 0.0230f)
					},
				},
				.exspecial{
					Node::Atk{
						.name = "Cavity Alert DMG",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 4.2100f, 0.3830f)
					},
					Node::Daze{
						.name = "Cavity Alert Daze",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 4.2100f, 0.1920f)
					},
					Node::Info{
						.name = "Cavity Alert Energy Cost",
						.type = Utils::EntryType::energy,
						.formula = Multiplier(Utils::EntryType::energy, LevelableSkill::special, 60.f, 0.f)
					},
					Node::Atk{
						.name = "Cavity Alert, Right Now! DMG",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.atk, LevelableSkill::special, 2.4140f, 0.2200f)
					},
					Node::Daze{
						.name = "Cavity Alert, Right Now! Daze",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 2.6700f, 0.1220f)
					},
					Node::Info{
						.name = "Cavity Alert, Right Now! Energy Cost",
						.type = Utils::EntryType::energy,
						.formula = Multiplier(Utils::EntryType::energy, LevelableSkill::special, 60.f, 0.f)
					},
					Node::Info{
						.name = "Sweet Scare Duration",
						.type = Utils::EntryType::energy,
						.formula = 16.f + skills.special * 2.f
					},
				},
				.chain{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::chain,
						.formula = Multiplier(combat.atk, LevelableSkill::chain, 5.9870f, 0.5450f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::chain,
						.formula = Multiplier(combat.impact, LevelableSkill::chain, 2.3290f, 0.1060f)
					},
				},
				.ultimate{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::chain,
						.formula = Multiplier(combat.atk, LevelableSkill::chain, 14.4320f, 1.3120f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::chain,
						.formula = Multiplier(combat.impact, LevelableSkill::chain, 2.8600f, 0.1300f)
					},
					Node::Info{
						.name = "Other Squad Members' Energy Gain",
						.type = Utils::EntryType::energy,
						.formula = 7.f + skills.chain * 1.5f,
					},
				},
				.mindscape4{
					Node::Info{
						.name = "Assist Follow-Up: Stuffed Hard Candy Shot DMG Increase",
						.type = Utils::EntryType::multiplier,
						.formula = m4Buff,
					},
					Node::Info{
						.name = "Assist Follow-Up: We Have Cookies DMG Increase",
						.type = Utils::EntryType::multiplier,
						.formula = m4Buff,
					},
				},
				.mindscape6{
					Node::CustomAtk{
						.name = "Additional Shell DMG",
						.formula = combat.atk * 3.f,
					},
				},
			},
		};
	},
};
