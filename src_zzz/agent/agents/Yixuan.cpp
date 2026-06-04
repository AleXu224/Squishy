#include "Yixuan.hpp"

#include "agent/setup.hpp"

const Agent::Data Agent::Datas::yixuan{
	.key{1371},
	.factionId = 10,
	.goodKey{"Yixuan"},
	.name{"Yixuan"},
	.baseStats{
		.baseHp = 673,
		.baseAtk = 126,
		.baseDef = 35,
		.baseImpact = 93,
		.baseAm = 92,
		.baseAp = 90,
		.coreStat1 = Stat::hp,
		.coreStat2 = Stat::cr,
		.rarity = 5,
		.attribute = Misc::Attribute::ether,
		.specialty = Misc::Specialty::rupture,
		.hpGrowth = 84.2519,
		.atkGrowth = 8.8572,
		.defGrowth = 4.8155,
		.hpUpgrade = {0.000, 462.000, 924.000, 1386.000, 1848.000, 2310.000},
		.atkUpgrade = {0.000, 45.000, 90.000, 134.000, 179.000, 224.000},
		.defUpgrade = {0.000, 24.000, 49.000, 73.000, 97.000, 122.000},
		.coreStat1Upgrade = {0.000, 140.000, 140.000, 280.000, 280.000, 420.000},
		.coreStat2Upgrade = {0.048, 0.048, 0.096, 0.096, 0.144, 0.144},
	},
	.setup = []() -> Data::Setup {
		auto multiplier1 = CoreMultiplier(true, {0.3f, 0.35f, 0.4f, 0.45f, 0.5f, 0.55f, 0.6f});

		auto coreSheerForceBuff = combat.hp * 0.1f;

		auto coreTeamCond = SpecialtyCountOthers{.specialty = Misc::Specialty::stun} >= 1
						 || SpecialtyCountOthers{.specialty = Misc::Specialty::support} >= 1
						 || SpecialtyCountOthers{.specialty = Misc::Specialty::defense} >= 1;

		auto exDmgBuff = Requires{
			.requirement = coreTeamCond && Requirement::enemyStunned,
			.ret = Constant{.value = 0.3f},
		};

		auto coreUltimateCond = IsActive("yixuanUltimateCond");
		auto coreUltimateBuff = Requires{
			.requirement = coreUltimateCond,
			.ret = Constant{.value = 0.4f},
		};

		auto m1Cr = Requires{
			.requirement = Requirement::mindscape1,
			.ret = Constant{.value = 0.1f},
		};

		auto m2ResDrecrease = Requires{
			.requirement = Requirement::mindscape2,
			.ret = Constant{.value = -0.15f},
		};

		auto m4Stacks = GetFloat("yixuanM4TranquilityStacks");
		auto m4Buff = Requires{
			.requirement = Requirement::mindscape4,
			.ret = m4Stacks * 0.3f,
		};

		auto m6Buff = Requires{
			.requirement = Requirement::mindscape6 && coreUltimateCond,
			.ret = Constant{.value = 0.2f},
		};

		return Data::Setup{
			.mods{
				.combat{
					.sheerForce = coreSheerForceBuff,
					.cr = m1Cr,
					.cd = coreUltimateBuff,
					.assistFollowup{
						.DMG = multiplier1,
					},
					.exspecial{
						.DMG = multiplier1,
					},
					.chain{
						.DMG = multiplier1,
					},
					.ultimate{
						.DMG = multiplier1,
					},
					.sheer{
						.DMG = m6Buff,
					},
				},
			},
			.opts{
				.core{
					Option::Boolean{
						.key = "yixuanUltimateCond",
						.name = "After activating Ultimate",
						.mods{
							.combat{
								.cd = coreUltimateBuff,
								.sheer{
									.DMG = m6Buff,
								},
							},
						},
					},
				},
				.mindscape4{
					Option::ValueSlider{
						.key = "yixuanM4TranquilityStacks",
						.name = "Tranquility Stacks",
						.values{0.f, 1.f, 2.f},
						.nodes{
							Node::Info{
								.name = "Next Cloud-Shaper and Ashen Ink Becomes Shadows DMG increase",
								.type = Utils::EntryType::multiplier,
								.formula = m4Buff,
							},
						},
					},
				},
			},
			.nodes{
				.basic{
					Node::Sheer{
						.name = "1st-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(Utils::EntryType::multiplier, LevelableSkill::basic, 0.4580f, 0.0420f)
					},
					Node::Sheer{
						.name = "2nd-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(Utils::EntryType::multiplier, LevelableSkill::basic, 0.6550f, 0.0600f)
					},
					Node::Sheer{
						.name = "3rd-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(Utils::EntryType::multiplier, LevelableSkill::basic, 0.8710f, 0.0800f)
					},
					Node::Sheer{
						.name = "4th-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(Utils::EntryType::multiplier, LevelableSkill::basic, 1.0480f, 0.0960f)
					},
					Node::Sheer{
						.name = "5th-Hit DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(Utils::EntryType::multiplier, LevelableSkill::basic, 1.1370f, 0.1040f)
					},
					Node::Daze{
						.name = "1st-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.2860f, 0.0130f)
					},
					Node::Daze{
						.name = "2nd-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.6890f, 0.0320f)
					},
					Node::Daze{
						.name = "3rd-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 0.8580f, 0.0390f)
					},
					Node::Daze{
						.name = "4th-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 1.1180f, 0.0510f)
					},
					Node::Daze{
						.name = "5th-Hit Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 1.0570f, 0.0490f)
					},
					Node::Sheer{
						.name = "Ink‐Veil Cloud Coalescence Total DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(Utils::EntryType::multiplier, LevelableSkill::basic, 2.3470f, 0.2140f)
					},
					Node::Daze{
						.name = "Ink‐Veil Cloud Coalescence Total Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 2.9340f, 0.1340f)
					},
					Node::Sheer{
						.name = "Auric Array Total DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(Utils::EntryType::multiplier, LevelableSkill::basic, 3.0520f, 0.2780f),
						.modifier{
							.DMG = multiplier1,
						},
					},
					Node::Daze{
						.name = "Auric Array Total Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 2.9340f, 0.1340f)
					},
					Node::Sheer{
						.name = "Qingming Eruption DMG",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(Utils::EntryType::multiplier, LevelableSkill::basic, 1.1060f, 0.1010f),
						.modifier{
							.DMG = multiplier1,
						},
					},
					Node::Daze{
						.name = "Qingming Eruption Daze",
						.source = Misc::AttackSource::basic,
						.formula = Multiplier(combat.impact, LevelableSkill::basic, 1.0630f, 0.0490f)
					},
				},
				.dashAttack{
					Node::Sheer{
						.name = "DMG",
						.source = Misc::AttackSource::dashAttack,
						.formula = Multiplier(Utils::EntryType::multiplier, LevelableSkill::dodge, 0.4990f, 0.0460f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::dashAttack,
						.formula = Multiplier(combat.impact, LevelableSkill::dodge, 0.3120f, 0.0150f)
					},
				},
				.dodgeCounter{
					Node::Sheer{
						.name = "DMG",
						.source = Misc::AttackSource::dodgeCounter,
						.formula = Multiplier(Utils::EntryType::multiplier, LevelableSkill::dodge, 2.1960f, 0.2000f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::dodgeCounter,
						.formula = Multiplier(combat.impact, LevelableSkill::dodge, 2.3650f, 0.1080f)
					},
				},
				.quickAssist{
					Node::Sheer{
						.name = "DMG",
						.source = Misc::AttackSource::quickAssist,
						.formula = Multiplier(Utils::EntryType::multiplier, LevelableSkill::assist, 1.0120f, 0.0920f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::quickAssist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 0.6330f, 0.0290f)
					},
				},
				.defensiveAssist{
					Node::Daze{
						.name = "Light Defensive Daze",
						.source = Misc::AttackSource::defensiveAssist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 2.7130f, 0.1240f)
					},
					Node::Daze{
						.name = "Heavy Defensive Daze",
						.source = Misc::AttackSource::defensiveAssist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 3.4280f, 0.1560f)
					},
					Node::Daze{
						.name = "Chain Defensive Daze",
						.source = Misc::AttackSource::defensiveAssist,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 1.6680f, 0.0760f)
					},
				},
				.assistFollowup{
					Node::Sheer{
						.name = "DMG",
						.source = Misc::AttackSource::assistFollowup,
						.formula = Multiplier(Utils::EntryType::multiplier, LevelableSkill::assist, 2.9700f, 0.2700f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::assistFollowup,
						.formula = Multiplier(combat.impact, LevelableSkill::assist, 3.2290f, 0.1470f)
					},
				},
				.special{
					Node::Sheer{
						.name = "DMG",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(Utils::EntryType::multiplier, LevelableSkill::special, 0.5210f, 0.0480f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::special,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 0.6510f, 0.0300f)
					},
				},
				.exspecial{
					Node::Sheer{
						.name = "DMG",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(Utils::EntryType::multiplier, LevelableSkill::special, 3.0030f, 0.2730f),
						.modifier{
							.enemy{
								.resistance{
									.ether = m2ResDrecrease,
								},
							},
						},
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 1.7290f, 0.0790f)
					},
					Node::Sheer{
						.name = "Additional DMG on Full Charge",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(Utils::EntryType::multiplier, LevelableSkill::special, 1.4960f, 0.1360f),
						.modifier{
							.enemy{
								.resistance{
									.ether = m2ResDrecrease,
								},
							},
						},
					},
					Node::Daze{
						.name = "Additional Daze on Full Charge",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 0.7580f, 0.0350f)
					},
					Node::Info{
						.name = "Adrenaline Cost",
						.type = Utils::EntryType::energy,
						.formula = Multiplier(Utils::EntryType::energy, LevelableSkill::special, 40.f, 0.f)
					},
					Node::Sheer{
						.name = "EX Special Attack: Celestial Cloud Blitz - Break Total DMG",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(Utils::EntryType::multiplier, LevelableSkill::special, 3.7060f, 0.3370f),
						.modifier{
							.enemy{
								.resistance{
									.ether = m2ResDrecrease,
								},
							},
						},
					},
					Node::Daze{
						.name = "EX Special Attack: Celestial Cloud Blitz - Break Total Daze",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 2.9440f, 0.1340f)
					},
					Node::Sheer{
						.name = "EX Special Attack: Qingming Eruption - Break Total DMG",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(Utils::EntryType::multiplier, LevelableSkill::special, 4.2640f, 0.3880f),
						.modifier{
							.enemy{
								.resistance{
									.ether = m2ResDrecrease,
								},
							},
						},
					},
					Node::Daze{
						.name = "EX Special Attack: Qingming Eruption - Break Total Daze",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 2.8530f, 0.1300f)
					},
					Node::Sheer{
						.name = "Cloud-Shaper Total DMG During Charge",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(Utils::EntryType::multiplier, LevelableSkill::special, 6.7180f, 0.6110f),
						.modifier{
							.DMG = exDmgBuff + m4Buff,
							.enemy{
								.resistance{
									.ether = m2ResDrecrease,
								},
							},
						},
					},
					Node::Daze{
						.name = "Cloud-Shaper Total Daze During Charge",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 4.8000f, 0.2190f)
					},
					Node::Info{
						.name = "Cloud-Shaper Adrenaline Cost to Use",
						.type = Utils::EntryType::energy,
						.formula = Multiplier(Utils::EntryType::energy, LevelableSkill::special, 40.f, 0.f)
					},
					Node::Info{
						.name = "Cloud-Shaper Initial Consumption",
						.type = Utils::EntryType::energy,
						.formula = Multiplier(Utils::EntryType::energy, LevelableSkill::special, 20.f, 0.f)
					},
					Node::Info{
						.name = "Cloud-Shaper Consumption Per Second",
						.type = Utils::EntryType::energy,
						.formula = Multiplier(Utils::EntryType::energy, LevelableSkill::special, 20.f, 0.f)
					},
					Node::Sheer{
						.name = "Ashen Ink Becomes Shadows DMG",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(Utils::EntryType::multiplier, LevelableSkill::special, 2.3430f, 0.2130f),
						.modifier{
							.DMG = exDmgBuff + m4Buff,
							.enemy{
								.resistance{
									.ether = m2ResDrecrease,
								},
							},
						},
					},
					Node::Daze{
						.name = "Ashen Ink Becomes Shadows Daze",
						.source = Misc::AttackSource::exspecial,
						.formula = Multiplier(combat.impact, LevelableSkill::special, 1.4210f, 0.0650f)
					},
				},
				.chain{
					Node::Sheer{
						.name = "DMG",
						.source = Misc::AttackSource::chain,
						.formula = Multiplier(Utils::EntryType::multiplier, LevelableSkill::chain, 5.3310f, 0.4850f)
					},
					Node::Daze{
						.name = "Daze",
						.source = Misc::AttackSource::chain,
						.formula = Multiplier(combat.impact, LevelableSkill::chain, 2.2740f, 0.1040f)
					},
				},
				.ultimate{
					Node::Sheer{
						.name = "Qingming Skyshade DMG",
						.source = Misc::AttackSource::ultimate,
						.formula = Multiplier(Utils::EntryType::multiplier, LevelableSkill::chain, 18.5340f, 1.6850f),
						.modifier{
							.enemy{
								.resistance{
									.ether = m2ResDrecrease,
								},
							},
						},
					},
					Node::Daze{
						.name = "Qingming Skyshade Daze",
						.source = Misc::AttackSource::ultimate,
						.formula = Multiplier(combat.impact, LevelableSkill::chain, 4.0710f, 0.1860f)
					},
					Node::Sheer{
						.name = "Endless Talisman Suppression DMG",
						.source = Misc::AttackSource::ultimate,
						.formula = Multiplier(Utils::EntryType::multiplier, LevelableSkill::chain, 14.6620f, 1.3330f),
						.modifier{
							.enemy{
								.resistance{
									.ether = m2ResDrecrease,
								},
							},
						},
					},
					Node::Daze{
						.name = "Endless Talisman Suppression Daze",
						.source = Misc::AttackSource::ultimate,
						.formula = Multiplier(combat.impact, LevelableSkill::chain, 2.4940f, 0.1140f)
					},
				},
				.core{
					Node::Sheer{
						.name = "Lightning Strike DMG",
						.attribute = Misc::Attribute::ether,
						.formula = Requires{
							.requirement = coreTeamCond,
							.ret = Constant{.value = 2.25f},
						},
					},
				},
				.mindscape1{
					Node::Mods{
						.mods{
							.combat{
								.cr = m1Cr,
							},
						},
					},
					Node::Sheer{
						.name = "Extra Lightning Strike DMG",
						.attribute = Misc::Attribute::ether,
						.formula = Requires{
							.requirement = Requirement::mindscape1,
							.ret = Constant{.value = 0.5f},
						},
					},
				},
				.mindscape2{
					Node::Sheer{
						.name = "Endless Talisman Suppression - Break DMG",
						.source = Misc::AttackSource::exspecial,
						.formula = Requires{
							.requirement = Requirement::mindscape2,
							.ret = Constant{.value = 12.f},
						},
						.modifier{
							.enemy{
								.resistance{
									.ether = m2ResDrecrease,
								},
							},
						},
					},
				},
			},
		};
	},
};
