#include "Alyosha.hpp"

#include "character/setup.hpp"

const Character::Data Character::Datas::alyosha{
	.key{10000148},
	.goodKey{"Alyosha"},
	.name{"Alyosha"},
	.baseStats{
		.baseHp = 1002.970,
		.baseAtk = 22.260,
		.baseDef = 58.942,
		.ascensionStat = Stat::er,
		.rarity = 4,
		.c3Talent = ConstellationTalent::skill,
		.c5Talent = ConstellationTalent::burst,
		.element = Misc::Element::electro,
		.weaponType = Misc::WeaponType::polearm,
		.hpCurve = Curves::CharacterGrow::HP_S4,
		.atkCurve = Curves::CharacterGrow::ATTACK_S4,
		.defCurve = Curves::CharacterGrow::HP_S4,
		.hpUpgrade = {0.000, 749.270, 1281.646, 1991.481, 2523.857, 3056.233, 3588.609},
		.atkUpgrade = {0.000, 16.630, 28.446, 44.200, 56.016, 67.832, 79.648},
		.defUpgrade = {0.000, 44.033, 75.319, 117.034, 148.320, 179.606, 210.893},
		.ascensionStatUpgrade = {0.000, 0.000, 0.067, 0.133, 0.133, 0.200, 0.267},
	},
	.setup = []() -> Data::Setup {
		auto radianceStellarConduct = IsActiveTeam("radianceStellarConduct");

		auto huntersMarkCond = IsActive("huntersMarkCond");
		auto huntersMarkMultiplier = Multiplier(Utils::EntryType::multiplier, LevelableTalent::skill, {0.1166, 0.1272, 0.1378, 0.1484, 0.1590, 0.1696, 0.1802, 0.1908, 0.2014, 0.2120, 0.2247, 0.2374, 0.2502, 0.2629, 0.2756});
		auto huntersMarkBuff = Requires{
			.requirement = huntersMarkCond,
			.ret = huntersMarkMultiplier,
		};

		auto a4Buff = Requires{
			.requirement = Requirement::passive2,
			.ret = Min{
				.val1 = total.er * ConstantFlat{.value = 100.f} * 0.0035f,
				.val2 = Constant{.value = 0.7f},
			}
		};

		auto p3Buff = Requires{
			.requirement = Requirement::passive3 && radianceStellarConduct && huntersMarkCond,
			.ret = Constant{.value = 0.2f},
		};

		auto c6HuntersMarkBuff = Requires{
			.requirement = Requirement::constellation6,
			.ret = huntersMarkBuff,
		};
		auto c6EmBuff = Requires{
			.requirement = Requirement::constellation6 && huntersMarkCond,
			.ret = ConstantFlat{.value = 100.f},
		};

		return Data::Setup{
			.mods{
				.preMod{
					.skill{
						.DMG = a4Buff,
					},
					.burst{
						.DMG = a4Buff,
					},
				},
				.activePreMod{
					.atk_ = huntersMarkBuff + c6HuntersMarkBuff,
					.em = c6EmBuff,
					.stellarConduct{
						.DMG = p3Buff,
					},
				},
			},
			.opts{
				.skill{
					Option::Boolean{
						.key = "huntersMarkCond",
						.name = "Hunter's Mark active",
						.teamBuff = true,
						.mods{
							.activePreMod{
								.atk_ = huntersMarkBuff + c6HuntersMarkBuff,
								.em = c6EmBuff,
								.stellarConduct{
									.DMG = p3Buff,
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
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4782, 0.5171, 0.5560, 0.6116, 0.6505, 0.6950, 0.7562, 0.8173, 0.8785, 0.9452, 1.0119, 1.0786, 1.1454, 1.2121, 1.2788}),
					},
					Node::Atk{
						.name = "2-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4816, 0.5208, 0.5600, 0.6160, 0.6552, 0.7000, 0.7616, 0.8232, 0.8848, 0.9520, 1.0192, 1.0864, 1.1536, 1.2208, 1.2880}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.3423, 0.3701, 0.3980, 0.4378, 0.4657, 0.4975, 0.5413, 0.5851, 0.6288, 0.6766, 0.7244, 0.7721, 0.8199, 0.8676, 0.9154})
								 + Multiplier(total.atk, LevelableTalent::normal, {0.3182, 0.3441, 0.3700, 0.4070, 0.4329, 0.4625, 0.5032, 0.5439, 0.5846, 0.6290, 0.6734, 0.7178, 0.7622, 0.8066, 0.8510}),
					},
					Node::Atk{
						.name = "4-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.7585, 0.8203, 0.8820, 0.9702, 1.0319, 1.1025, 1.1995, 1.2965, 1.3936, 1.4994, 1.6052, 1.7111, 1.8169, 1.9228, 2.0286}),
					},
				},
				.charged{
					Node::Atk{
						.name = "Charged Attack DMG",
						.source = Misc::AttackSource::charged,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.1103, 1.2006, 1.2910, 1.4201, 1.5105, 1.6137, 1.7558, 1.8978, 2.0398, 2.1947, 2.3496, 2.5045, 2.6595, 2.8144, 2.9693}),
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
						.name = "Low/High Plunge DMG",
						.source = Misc::AttackSource::plunge,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.2784, 1.3824, 1.4865, 1.6351, 1.7392, 1.8581, 2.0216, 2.1851, 2.3486, 2.5270, 2.7054, 2.8838, 3.0622, 3.2405, 3.4189}),
					},
					Node::Atk{
						.name = "Low/High Plunge DMG",
						.source = Misc::AttackSource::plunge,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.5968, 1.7267, 1.8567, 2.0424, 2.1723, 2.3209, 2.5251, 2.7293, 2.9336, 3.1564, 3.3792, 3.6020, 3.8248, 4.0476, 4.2704}),
					},
				},
				.skill{
					Node::Atk{
						.name = "Press DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {2.8672, 3.0822, 3.2973, 3.5840, 3.7990, 4.0141, 4.3008, 4.5875, 4.8742, 5.1610, 5.4477, 5.7344, 6.0928, 6.4512, 6.8096}),
					},
					Node::Atk{
						.name = "Hold DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {3.5840, 3.8528, 4.1216, 4.4800, 4.7488, 5.0176, 5.3760, 5.7344, 6.0928, 6.4512, 6.8096, 7.1680, 7.6160, 8.0640, 8.5120}),
					},
					Node::Info{
						.name = "CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000}),
					},
					Node::Info{
						.name = "Hunter's Mark Duration",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000}),
					},
					Node::Info{
						.name = "Hunter's Precision ATK Bonus",
						.type = Utils::EntryType::multiplier,
						.formula = huntersMarkMultiplier,
					},
					Node::Info{
						.name = "Hunter's Precision Duration",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000}),
					},
				},
				.burst{
					Node::Atk{
						.name = "Fulgurite Hunting Field DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.atk, LevelableTalent::burst, {0.7496, 0.8058, 0.8620, 0.9370, 0.9932, 1.0494, 1.1244, 1.1994, 1.2743, 1.3493, 1.4242, 1.4992, 1.5929, 1.6866, 1.7803}),
					},
					Node::Atk{
						.name = "Tugarin DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.atk, LevelableTalent::burst, {0.5022, 0.5399, 0.5776, 0.6278, 0.6655, 0.7031, 0.7533, 0.8036, 0.8538, 0.9040, 0.9542, 1.0045, 1.0672, 1.1300, 1.1928}),
					},
					Node::Info{
						.name = "Duration",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::burst, {14.0000, 14.0000, 14.0000, 14.0000, 14.0000, 14.0000, 14.0000, 14.0000, 14.0000, 14.0000, 14.0000, 14.0000, 14.0000, 14.0000, 14.0000}),
					},
					Node::Info{
						.name = "CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::burst, {18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000}),
					},
					Node::Info{
						.name = "Energy Cost",
						.type = Utils::EntryType::energy,
						.formula = Multiplier(Utils::EntryType::energy, LevelableTalent::burst, {70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000}),
					},
				},
				.passive1{
					Node::Heal{
						.name = "Tugarin Heal",
						.formula = Requires{
							.requirement = Requirement::passive1,
							.ret = total.atk * 1.2f,
						},
					},
				},
				.passive2{
					Node::Mods{
						.mods{
							.preMod{
								.skill{
									.DMG = a4Buff,
								},
								.burst{
									.DMG = a4Buff,
								},
							},
						},
					},
				},
				.constellation4{
					Node::Heal{
						.name = "Heal",
						.formula = Requires{
							.requirement = Requirement::constellation4,
							.ret = total.atk * 0.6f,
						},
					},
				},
			},
		};
	},
};
