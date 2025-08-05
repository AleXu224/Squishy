#include "Flins.hpp"

#include "character/setup.hpp"
#include "formula/teamCharacter.hpp"
#include "node/directLunarChargedNode.hpp"

const Character::Data Character::Datas::flins{
	.key{10000120},
	.goodKey{"Flins"},
	.name{"Flins"},
	.baseStats{
		.baseHp = 972.386,
		.baseAtk = 27.371,
		.baseDef = 62.941,
		.ascensionStat = Stat::cd,
		.rarity = 5,
		.c3Talent = ConstellationTalent::burst,
		.c5Talent = ConstellationTalent::skill,
		.element = Misc::Element::electro,
		.weaponType = Misc::WeaponType::polearm,
		.hpCurve = Curves::CharacterGrow::HP_S5,
		.atkCurve = Curves::CharacterGrow::ATTACK_S5,
		.defCurve = Curves::CharacterGrow::HP_S5,
		.hpUpgrade = {0.000, 833.733, 1426.123, 2215.976, 2808.365, 3400.755, 3993.144},
		.atkUpgrade = {0.000, 23.467, 40.141, 62.372, 79.046, 95.720, 112.394},
		.defUpgrade = {0.000, 53.968, 92.313, 143.440, 181.786, 220.131, 258.476},
		.ascensionStatUpgrade = {0.000, 0.000, 0.096, 0.192, 0.192, 0.288, 0.384},
	},
	.setup = []() -> Data::Setup {
		auto moonsignLevel = TeamMoonsignLevel{};

		auto a1Buff = Requires{
			moonsignLevel >= 2 && Requirement::passive1,
			Constant(0.2f),
		};

		auto a4Buff = Requires{
			Requirement::passive2,
			IfElse{
				Requirement::constellation4,
				Min(0.1f * total.atk, 220.f),
				Min(0.08f * total.atk, 160.f),
			},
		};

		auto p3Buff = Min(0.007f * total.atk / 100.f, 0.14f);

		auto c2Cond = IsActive("flinsC2Cond");
		auto c2Buff = Requires{
			c2Cond && moonsignLevel >= 2 && Requirement::constellation2,
			Constant(-0.25f),
		};

		auto c4Buff = Requires{
			Requirement::constellation4,
			Constant(0.2f)
		};

		auto c6BuffSelf = Requires{
			Requirement::constellation6,
			Constant(0.3f),
		};
		auto c6BuffTeam = Requires{
			Requirement::constellation6 && moonsignLevel >= 2,
			Constant(0.1f),
		};

		return Data::Setup{
			.mods{
				.preMod{
					.atk_ = c4Buff,
					.lunarCharged{
						.DMG = a1Buff,
						.elevation = c6BuffSelf,
					},
				},
				.postMod{
					.em = a4Buff,
				},
				.teamPreMod{
					.lunarCharged{
						.elevation = c6BuffTeam,
					},
				},
				.enemy{
					.resistance{
						.electro = c2Buff,
					},
				},
				.moonsignLevel = ConstantInt(1),
			},
			.opts{
				.constellation2{
					Option::Boolean{
						.key = "flinsC2Cond",
						.name = "Flins is on the field and his Electro attacks hit an opponent",
						.teamBuff = true,
						.mods{
							.enemy{
								.resistance{
									.electro = c2Buff,
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
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4473, 0.4837, 0.5201, 0.5721, 0.6085, 0.6501, 0.7073, 0.7645, 0.8217, 0.8841, 0.9465, 1.0089, 1.0713, 1.1338, 1.1962}),
					},
					Node::Atk{
						.name = "2-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4515, 0.4882, 0.5250, 0.5775, 0.6142, 0.6562, 0.7140, 0.7717, 0.8295, 0.8925, 0.9555, 1.0185, 1.0815, 1.1445, 1.2075}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.5592, 0.6047, 0.6502, 0.7153, 0.7608, 0.8128, 0.8843, 0.9558, 1.0274, 1.1054, 1.1834, 1.2614, 1.3395, 1.4175, 1.4955}),
					},
					Node::Atk{
						.name = "4-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.3204, 0.3465, 0.3725, 0.4098, 0.4359, 0.4657, 0.5067, 0.5476, 0.5886, 0.6333, 0.6780, 0.7227, 0.7674, 0.8121, 0.8569}),
					},
					Node::Atk{
						.name = "5-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.7679, 0.8305, 0.8930, 0.9823, 1.0448, 1.1162, 1.2144, 1.3127, 1.4109, 1.5180, 1.6252, 1.7323, 1.8395, 1.9467, 2.0538}),
					},
				},
				.charged{
					Node::Atk{
						.name = "Charged Attack DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.0303, 1.1141, 1.1980, 1.3178, 1.4017, 1.4975, 1.6293, 1.7611, 1.8928, 2.0366, 2.1804, 2.3241, 2.4679, 2.6116, 2.7554}),
					},
					Node::Info{
						.name = "Charged Attack Stamina Cost",
						.formula = Multiplier(LevelableTalent::normal, {25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000}),
					},
				},
				.plunge{
					Node::Atk{
						.name = "Plunge DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.6393, 0.6914, 0.7434, 0.8177, 0.8698, 0.9293, 1.0110, 1.0928, 1.1746, 1.2638, 1.3530, 1.4422, 1.5314, 1.6206, 1.7098}),
					},
					Node::Atk{
						.name = "Low Plunge DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.2784, 1.3824, 1.4865, 1.6351, 1.7392, 1.8581, 2.0216, 2.1851, 2.3486, 2.5270, 2.7054, 2.8838, 3.0622, 3.2405, 3.4189}),
					},
					Node::Atk{
						.name = "High Plunge DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.5968, 1.7267, 1.8567, 2.0424, 2.1723, 2.3209, 2.5251, 2.7293, 2.9336, 3.1564, 3.3792, 3.6020, 3.8248, 4.0476, 4.2704}),
					},
				},
				.skill{
					Node::Atk{
						.name = "1-Hit DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.5825, 0.6262, 0.6699, 0.7281, 0.7718, 0.8155, 0.8737, 0.9320, 0.9902, 1.0485, 1.1067, 1.1650, 1.2378, 1.3106, 1.3834}),
					},
					Node::Atk{
						.name = "2-Hit DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.5880, 0.6321, 0.6762, 0.7350, 0.7791, 0.8232, 0.8820, 0.9408, 0.9996, 1.0584, 1.1172, 1.1760, 1.2494, 1.3229, 1.3964}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.7283, 0.7829, 0.8375, 0.9103, 0.9649, 1.0196, 1.0924, 1.1652, 1.2380, 1.3109, 1.3837, 1.4565, 1.5475, 1.6386, 1.7296}),
					},
					Node::Atk{
						.name = "4-Hit DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.4173, 0.4485, 0.4798, 0.5216, 0.5529, 0.5842, 0.6259, 0.6676, 0.7093, 0.7511, 0.7928, 0.8345, 0.8867, 0.9388, 0.9910}),
					},
					Node::Atk{
						.name = "5-Hit DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {1.0001, 1.0751, 1.1501, 1.2501, 1.3251, 1.4002, 1.5002, 1.6002, 1.7002, 1.8002, 1.9002, 2.0002, 2.1252, 2.2503, 2.3753}),
					},
					Node::Atk{
						.name = "Charged Attack DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {1.1496, 1.2358, 1.3220, 1.4370, 1.5232, 1.6094, 1.7244, 1.8394, 1.9543, 2.0693, 2.1842, 2.2992, 2.4429, 2.5866, 2.7303}),
					},
					Node::Atk{
						.name = "Northland Spearstorm DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {1.7840, 1.9178, 2.0516, 2.2300, 2.3638, 2.4976, 2.6760, 2.8544, 3.0328, 3.2112, 3.3896, 3.5680, 3.7910, 4.0140, 4.2370}),
					},
					Node::Info{
						.name = "Northland Spearstorm CD",
						.formula = Multiplier(LevelableTalent::skill, {6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000}),
					},
					Node::Info{
						.name = "Manifest Flame Duration",
						.formula = Multiplier(LevelableTalent::skill, {10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000}),
					},
					Node::Info{
						.name = "Skill CD",
						.formula = Multiplier(LevelableTalent::skill, {16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000}),
					},
				},
				.burst{
					Node::Atk{
						.name = "Initial Skill DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.atk, LevelableTalent::burst, {2.5984, 2.7933, 2.9882, 3.2480, 3.4429, 3.6378, 3.8976, 4.1574, 4.4173, 4.6771, 4.9370, 5.1968, 5.5216, 5.8464, 6.1712}),
					},
					Node::DirectLunarCharged{
						.name = "Lunar-Charged DMG Per Phase",
						.formula = Multiplier(total.atk, LevelableTalent::burst, {0.2598, 0.2793, 0.2988, 0.3248, 0.3443, 0.3638, 0.3898, 0.4157, 0.4417, 0.4677, 0.4937, 0.5197, 0.5522, 0.5846, 0.6171}),
					},
					Node::DirectLunarCharged{
						.name = "Final Phase Lunar-Charged DMG",
						.formula = Multiplier(total.atk, LevelableTalent::burst, {1.2992, 1.3966, 1.4941, 1.6240, 1.7214, 1.8189, 1.9488, 2.0787, 2.2086, 2.3386, 2.4685, 2.5984, 2.7608, 2.9232, 3.0856}),
					},
					Node::Info{
						.name = "Energy Cost",
						.formula = Multiplier(LevelableTalent::burst, {70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000}),
					},
					Node::Info{
						.name = "CD",
						.formula = Multiplier(LevelableTalent::burst, {18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000}),
					},
					Node::DirectLunarCharged{
						.name = "Thunderous Symphony DMG",
						.formula = Multiplier(total.atk, LevelableTalent::burst, {0.7795, 0.8380, 0.8964, 0.9744, 1.0329, 1.0913, 1.1693, 1.2472, 1.3252, 1.4031, 1.4811, 1.5590, 1.6565, 1.7539, 1.8514}),
					},
					Node::DirectLunarCharged{
						.name = "Thunderous Symphony Additional DMG",
						.formula = Multiplier(total.atk, LevelableTalent::burst, {1.1693, 1.2570, 1.3447, 1.4616, 1.5493, 1.6370, 1.7539, 1.8708, 1.9878, 2.1047, 2.2216, 2.3386, 2.4847, 2.6309, 2.7770}),
					},
					Node::Info{
						.name = "Thunderous Symphony Energy Cost",
						.formula = Multiplier(LevelableTalent::burst, {30.0000, 30.0000, 30.0000, 30.0000, 30.0000, 30.0000, 30.0000, 30.0000, 30.0000, 30.0000, 30.0000, 30.0000, 30.0000, 30.0000, 30.0000}),
					},
				},
				.passive1{
					Node::Mods{
						.mods{
							.preMod{
								.lunarCharged{
									.DMG = a1Buff,
								},
							},
						},
					},
				},
				.passive2{
					Node::Mods{
						.mods{
							.postMod{.em = a4Buff},
						},
					},
				},
				.passive3{
					Node::Mods{
						.mods{
							.moonsignLevel = ConstantInt(1)
						},
					},
				},
				.constellation2{
					Node::DirectLunarCharged{
						.name = "DMG",
						.formula = 0.5f * total.atk,
					},
				},
				.constellation4{
					Node::Mods{
						.mods{
							.preMod{.atk_ = c4Buff},
						},
					},
				},
				.constellation6{
					Node::Mods{
						.mods{
							.preMod{
								.lunarCharged{.elevation = c6BuffSelf},
							},
							.teamPreMod{
								.lunarCharged{.elevation = c6BuffTeam},
							},
						},
					},
				},
			},
		};
	},
};
