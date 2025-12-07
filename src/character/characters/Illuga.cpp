#include "Illuga.hpp"

#include "character/setup.hpp"

const Character::Data Character::Datas::illuga{
	.key{10000127},
	.goodKey{"Illuga"},
	.name{"Illuga"},
	.baseStats{
		.baseHp = 1002.970,
		.baseAtk = 16.027,
		.baseDef = 68.212,
		.ascensionStat = Stat::em,
		.rarity = 4,
		.c3Talent = ConstellationTalent::burst,
		.c5Talent = ConstellationTalent::skill,
		.element = Misc::Element::geo,
		.weaponType = Misc::WeaponType::polearm,
		.hpCurve = Curves::CharacterGrow::HP_S4,
		.atkCurve = Curves::CharacterGrow::ATTACK_S4,
		.defCurve = Curves::CharacterGrow::HP_S4,
		.hpUpgrade = {0.000, 749.270, 1281.646, 1991.481, 2523.857, 3056.233, 3588.609},
		.atkUpgrade = {0.000, 11.973, 20.481, 31.824, 40.332, 48.839, 57.346},
		.defUpgrade = {0.000, 50.958, 87.165, 135.441, 171.648, 207.855, 244.062},
		.ascensionStatUpgrade = {0.000, 0.000, 24.000, 48.000, 48.000, 72.000, 96.000},
	},
	.setup = []() -> Data::Setup {
		auto burstCond = IsActive("illugaBurstActive");
		auto burstDmgIncrease = Requires{
			.requirement = burstCond,
			.ret = Multiplier(total.em, LevelableTalent::burst, {0.3360, 0.3612, 0.3864, 0.4200, 0.4452, 0.4704, 0.5040, 0.5376, 0.5712, 0.6048, 0.6384, 0.6720, 0.7140, 0.7560, 0.7980}),
		};
		auto burstLunarCrystallizeDmgIncrease = Requires{
			.requirement = burstCond,
			.ret = Multiplier(total.em, LevelableTalent::burst, {2.8240, 3.0358, 3.2476, 3.5300, 3.7418, 3.9536, 4.2360, 4.5184, 4.8008, 5.0832, 5.3656, 5.6480, 6.0010, 6.3540, 6.7070}),
		};

		auto a1Cond = IsActive("illugaA1Cond");
		auto a1CrBuff = Requires{
			.requirement = Requirement::passive1 && a1Cond,
			.ret = IfElse{
				.requirement = Requirement::constellation6,
				.trueVal = Constant(0.10f),
				.elseVal = Constant(0.05f),
			},
		};
		auto a1CdBuff = Requires{
			.requirement = Requirement::passive1 && a1Cond,
			.ret = IfElse{
				.requirement = Requirement::constellation6,
				.trueVal = Constant(0.30f),
				.elseVal = Constant(0.10f),
			},
		};

		auto a4Count = ElementCount{.element = Misc::Element::geo} + ElementCount{.element = Misc::Element::hydro};
		auto a4GeoDmgIncrease = Requires{
			.requirement = Requirement::passive2 && burstCond,
			.ret = IfElse{
				.requirement = a4Count >= 3,
				.trueVal = Constant(0.24f) * total.em,
				.elseVal = IfElse{
					.requirement = a4Count == 2,
					.trueVal = Constant(0.14f) * total.em,
					.elseVal = Constant(0.07f) * total.em,
				},
			},
		};
		auto a4LunarCrystallizeDmgIncrease = Requires{
			.requirement = Requirement::passive2 && burstCond,
			.ret = IfElse{
				.requirement = a4Count >= 3,
				.trueVal = Constant(2.f) * total.em,
				.elseVal = IfElse{
					.requirement = a4Count == 2,
					.trueVal = Constant(1.2f) * total.em,
					.elseVal = Constant(0.6f) * total.em,
				},
			},
		};

		auto c4Buff = Requires{
			.requirement = Requirement::constellation4 && burstCond,
			.ret = ConstantFlat(200.f),
		};


		return Data::Setup{
			.mods{
				.teamPreMod{
					.def = c4Buff,
					.geo{
						.additiveDMG = burstDmgIncrease,
						.critRate = a1CrBuff,
						.critDMG = a1CdBuff,
					},
					.lunarCrystallize{
						.additiveDMG = burstDmgIncrease + burstLunarCrystallizeDmgIncrease,
					},
				},
				.moonsignLevel = ConstantInt(1),
			},
			.opts{
				.burst{
					Option::Boolean{
						.key = "illugaBurstActive",
						.name = "Burst Active",
						.teamBuff = true,
						.mods{
							.teamPreMod{
								.def = c4Buff,
								.geo{
									.additiveDMG = burstDmgIncrease + a4GeoDmgIncrease,
								},
								.lunarCrystallize{
									.additiveDMG = burstDmgIncrease + burstLunarCrystallizeDmgIncrease + a4LunarCrystallizeDmgIncrease,
								},
							},
						},
					},
				},
				.passive1{
					Option::Boolean{
						.key = "illugaA1Cond",
						.name = "After using Elemental Skill or Elemental Burst",
						.teamBuff = true,
						.mods{
							.teamPreMod{
								.geo{
									.critRate = a1CrBuff,
									.critDMG = a1CdBuff,
								},
							},
						},
					},
				},
			},
			.nodes{
				.normal{
					Node::Atk{
						.name = "1-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4737, 0.5122, 0.5508, 0.6058, 0.6444, 0.6885, 0.7490, 0.8096, 0.8702, 0.9363, 1.0024, 1.0685, 1.1346, 1.2007, 1.2668}),
					},
					Node::Atk{
						.name = "2-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4853, 0.5248, 0.5643, 0.6207, 0.6602, 0.7053, 0.7674, 0.8294, 0.8915, 0.9592, 1.0269, 1.0946, 1.1624, 1.2301, 1.2978}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.3143, 0.3399, 0.3655, 0.4021, 0.4276, 0.4569, 0.4971, 0.5373, 0.5775, 0.6213, 0.6652, 0.7091, 0.7529, 0.7968, 0.8407}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.3143, 0.3399, 0.3655, 0.4021, 0.4276, 0.4569, 0.4971, 0.5373, 0.5775, 0.6213, 0.6652, 0.7091, 0.7529, 0.7968, 0.8407}),
					},
					Node::Atk{
						.name = "4-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.7628, 0.8249, 0.8870, 0.9757, 1.0377, 1.1087, 1.2063, 1.3038, 1.4014, 1.5078, 1.6143, 1.7207, 1.8271, 1.9336, 2.0400}),
					},
				},
				.charged{
					Node::Atk{
						.name = "Charged Attack DMG",
						.source = Misc::AttackSource::charged,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.1103, 1.2006, 1.2910, 1.4201, 1.5105, 1.6138, 1.7558, 1.8978, 2.0398, 2.1947, 2.3496, 2.5045, 2.6595, 2.8144, 2.9693}),
					},
					Node::Info{
						.name = "Charged Attack Stamina Cost",
						.type = Utils::EntryType::points,
						.formula = Multiplier(Utils::EntryType::points, LevelableTalent::normal, {25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000}),
					},
				},
				.plunge{
					Node::Atk{
						.name = "Plunge DMG",
						.source = Misc::AttackSource::plunge,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.6393, 0.6914, 0.7434, 0.8177, 0.8698, 0.9293, 1.0110, 1.0928, 1.1746, 1.2638, 1.3530, 1.4422, 1.5314, 1.6206, 1.7098}),
					},
					Node::Atk{
						.name = "Low Plunge DMG",
						.source = Misc::AttackSource::plunge,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.2784, 1.3824, 1.4865, 1.6351, 1.7392, 1.8581, 2.0216, 2.1851, 2.3486, 2.5270, 2.7054, 2.8838, 3.0622, 3.2405, 3.4189}),
					},
					Node::Atk{
						.name = "High Plunge DMG",
						.source = Misc::AttackSource::plunge,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.5968, 1.7267, 1.8567, 2.0424, 2.1723, 2.3209, 2.5251, 2.7293, 2.9336, 3.1564, 3.3792, 3.6020, 3.8248, 4.0476, 4.2704}),
					},
				},
				.skill{
					Node::Atk{
						.name = "Press DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.em, LevelableTalent::skill, {4.8256, 5.1875, 5.5494, 6.0320, 6.3939, 6.7558, 7.2384, 7.7210, 8.2035, 8.6861, 9.1686, 9.6512, 10.2544, 10.8576, 11.4608})
								 + Multiplier(total.def, LevelableTalent::skill, {2.4128, 2.5938, 2.7747, 3.0160, 3.1970, 3.3779, 3.6192, 3.8605, 4.1018, 4.3430, 4.5843, 4.8256, 5.1272, 5.4288, 5.7304}),
					},
					Node::Atk{
						.name = "Hold DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.em, LevelableTalent::skill, {6.0320, 6.4844, 6.9368, 7.5400, 7.9924, 8.4448, 9.0480, 9.6512, 10.2544, 10.8576, 11.4608, 12.0640, 12.8180, 13.5720, 14.3260})
								 + Multiplier(total.def, LevelableTalent::skill, {3.0160, 3.2422, 3.4684, 3.7700, 3.9962, 4.2224, 4.5240, 4.8256, 5.1272, 5.4288, 5.7304, 6.0320, 6.4090, 6.7860, 7.1630}),
					},
					Node::Info{
						.name = "CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000}),
					},
				},
				.burst{
					Node::Atk{
						.name = "Skill DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.em, LevelableTalent::burst, {8.2720, 8.8924, 9.5128, 10.3400, 10.9604, 11.5808, 12.4080, 13.2352, 14.0624, 14.8896, 15.7168, 16.5440, 17.5780, 18.6120, 19.6460})
								 + Multiplier(total.def, LevelableTalent::burst, {4.1360, 4.4462, 4.7564, 5.1700, 5.4802, 5.7904, 6.2040, 6.6176, 7.0312, 7.4448, 7.8584, 8.2720, 8.7890, 9.3060, 9.8230}),
					},
					Node::Info{
						.name = "Geo DMG Bonus",
						.type = Utils::EntryType::points,
						.optimizable = true,
						.formula = burstDmgIncrease,
					},
					Node::Info{
						.name = "Lunar-Crystallize Reaction DMG Bonus",
						.type = Utils::EntryType::points,
						.optimizable = true,
						.formula = burstLunarCrystallizeDmgIncrease,
					},
					Node::Info{
						.name = "Nightingale's Song Stacks Gained from Elemental Burst",
						.type = Utils::EntryType::points,
						.formula = Multiplier(Utils::EntryType::points, LevelableTalent::burst, {21.0000, 21.0000, 21.0000, 21.0000, 21.0000, 21.0000, 21.0000, 21.0000, 21.0000, 21.0000, 21.0000, 21.0000, 21.0000, 21.0000, 21.0000}),
					},
					Node::Info{
						.name = "Nightingale's Song Stacks Gained from Creating Geo Constructs",
						.type = Utils::EntryType::points,
						.formula = Multiplier(Utils::EntryType::points, LevelableTalent::burst, {5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000}),
					},
					Node::Info{
						.name = "Duration",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::burst, {20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000}),
					},
					Node::Info{
						.name = "CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::burst, {15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000}),
					},
					Node::Info{
						.name = "Energy Cost",
						.type = Utils::EntryType::energy,
						.formula = Multiplier(Utils::EntryType::energy, LevelableTalent::burst, {60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000}),
					},
				},
				.constellation2{
					Node::Atk{
						.name = "Gale DMG",
						.element = Misc::Element::geo,
						.formula = total.em * 4.f + total.def * 4.f,
					},
				},
			},
		};
	},
};
