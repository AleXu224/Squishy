#include "Ineffa.hpp"

#include "character/setup.hpp"
#include "node/directLunarChargedNode.hpp"

const Character::Data Character::Datas::ineffa{
	.key{10000116},
	.goodKey{"Ineffa"},
	.name{"Ineffa"},
	.baseStats{
		.baseHp = 981.920,
		.baseAtk = 25.696,
		.baseDef = 64.437,
		.ascensionStat = Stat::cr,
		.rarity = 5,
		.c3Talent = ConstellationTalent::skill,
		.c5Talent = ConstellationTalent::burst,
		.element = Misc::Element::electro,
		.weaponType = Misc::WeaponType::polearm,
		.hpCurve = Curves::CharacterGrow::HP_S5,
		.atkCurve = Curves::CharacterGrow::ATTACK_S5,
		.defCurve = Curves::CharacterGrow::HP_S5,
		.hpUpgrade = {0.000, 841.907, 1440.104, 2237.701, 2835.898, 3434.095, 4032.293},
		.atkUpgrade = {0.000, 22.030, 37.683, 58.554, 74.206, 89.859, 105.512},
		.defUpgrade = {0.000, 55.250, 94.507, 146.849, 186.106, 225.362, 264.619},
		.ascensionStatUpgrade = {0.000, 0.000, 0.048, 0.096, 0.096, 0.144, 0.192},
	},
	.setup = []() -> Data::Setup {
		auto a4Cond = IsActive("ineffaA4");
		auto a4Buff = Requires(a4Cond && Requirement::passive2, 0.06f * total.atk);

		auto p3Buff = Min(0.007f * total.atk / ConstantFlat(100.f), 0.14f);

		auto c1Cond = IsActive("ineffaC1");
		auto c1Buff = Requires{
			.requirement = Requirement::constellation1 && c1Cond,
			.ret = Min{
				0.025f * total.atk / ConstantFlat(100.f),
				0.5f,
			},
		};

		return Data::Setup{
			.mods{
				.teamPreMod{
					.lunarCharged{
						.DMG = c1Buff,
						.multiplicativeDMG = p3Buff,
					},
				},
				.teamPostMod{
					.em = a4Buff,
				},
				.moonsignLevel = ConstantInt(1),
			},
			.opts{
				.passive2{
					Option::Boolean{
						.key = "ineffaA4",
						.name = "After using Elemental Burst",
						.teamBuff = true,
						.mods{
							.teamPostMod{
								.em = a4Buff,
							},
						},
					},
				},
				.constellation1{
					Option::Boolean{
						.key = "ineffaC1",
						.name = "After using Elemental Skill",
						.teamBuff = true,
						.mods{
							.teamPreMod{
								.lunarCharged{
									.DMG = c1Buff,
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
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.3484, 0.3767, 0.4051, 0.4456, 0.4739, 0.5063, 0.5509, 0.5954, 0.6400, 0.6886, 0.7372, 0.7858, 0.8344, 0.8830, 0.9316}),
					},
					Node::Atk{
						.name = "2-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.3422, 0.3701, 0.3979, 0.4377, 0.4656, 0.4974, 0.5412, 0.5849, 0.6287, 0.6765, 0.7242, 0.7720, 0.8197, 0.8675, 0.9152}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.2276, 0.2461, 0.2646, 0.2911, 0.3096, 0.3307, 0.3599, 0.3890, 0.4181, 0.4498, 0.4816, 0.5133, 0.5451, 0.5768, 0.6086}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.2276, 0.2461, 0.2646, 0.2911, 0.3096, 0.3307, 0.3599, 0.3890, 0.4181, 0.4498, 0.4816, 0.5133, 0.5451, 0.5768, 0.6086}),
					},
					Node::Atk{
						.name = "4-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.5607, 0.6063, 0.6520, 0.7171, 0.7628, 0.8149, 0.8867, 0.9584, 1.0301, 1.1083, 1.1865, 1.2648, 1.3430, 1.4213, 1.4995}),
					},
				},
				.charged{
					Node::Atk{
						.name = "Charged Attack DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.9494, 1.0267, 1.1040, 1.2144, 1.2917, 1.3800, 1.5014, 1.6229, 1.7443, 1.8768, 2.0093, 2.1418, 2.2742, 2.4067, 2.5392}),
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
						.name = "Skill DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.8640, 0.9288, 0.9936, 1.0800, 1.1448, 1.2096, 1.2960, 1.3824, 1.4688, 1.5552, 1.6416, 1.7280, 1.8360, 1.9440, 2.0520}),
					},
					Node::Shield{
						.name = "Shield DMG Absorption",
						.element = Misc::Element::electro,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {2.2118, 2.3777, 2.5436, 2.7648, 2.9307, 3.0966, 3.3178, 3.5389, 3.7601, 3.9813, 4.2025, 4.4237, 4.7002, 4.9766, 5.2531})
								 + Multiplier(LevelableTalent::skill, {1386.6759, 1525.3628, 1675.6068, 1837.4082, 2010.7667, 2195.6826, 2392.1558, 2600.1860, 2819.7734, 3050.9182, 3293.6204, 3547.8796, 3813.6960, 4091.0698, 4380.0005}),
					},
					Node::Info{
						.name = "Shield Duration",
						.formula = Multiplier(LevelableTalent::skill, {20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000}),
					},
					Node::Atk{
						.name = "Birgitta Discharge DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.9600, 1.0320, 1.1040, 1.2000, 1.2720, 1.3440, 1.4400, 1.5360, 1.6320, 1.7280, 1.8240, 1.9200, 2.0400, 2.1600, 2.2800}),
					},
					Node::Info{
						.name = "Birgitta Duration",
						.formula = Multiplier(LevelableTalent::skill, {20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000}),
					},
					Node::Info{
						.name = "CD",
						.formula = Multiplier(LevelableTalent::skill, {16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000}),
					},
				},
				.burst{
					Node::Atk{
						.name = "Skill DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.atk, LevelableTalent::burst, {6.7680, 7.2756, 7.7832, 8.4600, 8.9676, 9.4752, 10.1520, 10.8288, 11.5056, 12.1824, 12.8592, 13.5360, 14.3820, 15.2280, 16.0740}),
					},
					Node::Info{
						.name = "CD",
						.formula = Multiplier(LevelableTalent::burst, {15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000}),
					},
					Node::Info{
						.name = "Energy Cost",
						.formula = Multiplier(LevelableTalent::burst, {60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000}),
					},
				},
				.passive1{
					Node::DirectLunarCharged{
						.name = "Birgitta Additional Attack",
						.formula = 0.65f * total.atk,
					},
				},
				.passive3{
					Node::Info{
						.name = "Lunar-Charged Base DMG Increase",
						.type = Utils::EntryType::multiplier,
						.optimizable = true,
						.formula = p3Buff,
					},
				},
				.constellation2{
					Node::DirectLunarCharged{
						.name = "Punishment Edict DMG",
						.formula = 3.f * total.atk,
					},
				},
				.constellation6{
					Node::DirectLunarCharged{
						.name = "DMG",
						.formula = 1.35f * total.atk,
					},
				},
			},
		};
	},
};
