#include "Zibai.hpp"

#include "character/setup.hpp"

const Character::Data Character::Datas::zibai{
	.key{10000126},
	.goodKey{"Zibai"},
	.name{"Zibai"},
	.baseStats{
		.baseHp = 1005.753,
		.baseAtk = 17.503,
		.baseDef = 74.488,
		.ascensionStat = Stat::cd,
		.rarity = 5,
		.c3Talent = ConstellationTalent::skill,
		.c5Talent = ConstellationTalent::burst,
		.element = Misc::Element::geo,
		.weaponType = Misc::WeaponType::sword,
		.hpCurve = Curves::CharacterGrow::HP_S5,
		.atkCurve = Curves::CharacterGrow::ATTACK_S5,
		.defCurve = Curves::CharacterGrow::HP_S5,
		.hpUpgrade = {0.000, 862.342, 1475.058, 2292.014, 2904.731, 3517.447, 4130.164},
		.atkUpgrade = {0.000, 15.006, 25.668, 39.884, 50.546, 61.209, 71.871},
		.defUpgrade = {0.000, 63.868, 109.249, 169.756, 215.136, 260.516, 305.897},
		.ascensionStatUpgrade = {0.000, 0.000, 0.096, 0.192, 0.192, 0.288, 0.384},
	},
	.setup = []() -> Data::Setup {
		auto a1Cond = IsActive("zibaiA1Cond");
		auto a1Buff = Requires{
			.requirement = Requirement::passive1 && a1Cond,
			.ret = IfElse{
				.requirement = Requirement::constellation2,
				.trueVal = total.def,
				.elseVal = Constant(0.4f) * total.def,
			},
		};

		auto a4DefBuff = Requires{
			.requirement = Requirement::passive2,
			.ret = 0.15f * ElementCountOthers{.element = Misc::Element::geo}
		};
		auto a4EmBuff = Requires{
			.requirement = Requirement::passive2,
			.ret = ConstantFlat(60.f) * ElementCountOthers{.element = Misc::Element::hydro}
		};

		auto p3Buff = Min{
			.val1 = total.def / ConstantFlat(100.f) * 0.007f,
			.val2 = 0.14f,
		};

		auto c1Cond = IsActive("zibaiC1Cond");
		auto c1Buff = Requires{
			.requirement = Requirement::constellation1 && c1Cond,
			.ret = Constant(2.f),
		};

		auto c2Cond = IsActive("zibaiC2Cond");
		auto c2Buff = Requires{
			.requirement = Requirement::constellation2 && c2Cond,
			.ret = Constant(0.5f),
		};

		auto c4Cond = IsActive("zibaiC4Cond");
		auto c4Buff = Requires{
			.requirement = Requirement::constellation4 && c4Cond,
			.ret = total.def * 0.6f,
		};

		auto c6Cond = GetFloat("zibaiC6Cond");
		auto c6Buff = Requires{
			.requirement = Requirement::constellation6,
			.ret = Constant(0.016f) * c6Cond,
		};

		return Data::Setup{
			.mods{
				.preMod{
					.def_ = a4DefBuff + c2Buff,
					.em = a4EmBuff,
				},
				.teamPreMod{
					.lunarCrystallize{
						.multiplicativeDMG = p3Buff,
					},
				},
				.moonsignLevel = ConstantInt(1),
			},
			.opts{
				.passive1{
					Option::Boolean{
						.key = "zibaiA1Cond",
						.name = "After casting the Elemental Skill or triggering Moondrift Harmony",
						.nodes{
							Node::Info{
								.name = "Spirit Steed's Stride DMG Increase",
								.type = Utils::EntryType::points,
								.formula = a1Buff,
							},
						},
					},
				},
				.constellation1{
					Option::Boolean{
						.key = "zibaiC1Cond",
						.name = "First hit of Spirit Steed's Charge",
						.nodes{
							Node::Info{
								.name = "Spirit Steed's Charge Lunar-Crystallize Reaction DMG",
								.type = Utils::EntryType::multiplier,
								.formula = c1Buff,
							},
						},
					},
				},
				.constellation2{
					Option::Boolean{
						.key = "zibaiC2Cond",
						.name = "In the Lunar Phase Shift mode",
						.mods{
							.preMod{
								.def_ = c2Buff,
							},
						},
					},
				},
				.constellation4{
					Option::Boolean{
						.key = "zibaiC4Cond",
						.name = "Scattermoon Splendor",
						.nodes{
							Node::Info{
								.name = "Spirit Steed's Stride DMG Increase",
								.type = Utils::EntryType::points,
								.formula = c4Buff,
							},
						},
					},
				},
				.constellation6{
					Option::ValueList{
						.key = "zibaiC6Cond",
						.prefix = "Points consumed above 70",
						.values{5, 10, 15, 20, 25, 30},
						.nodes{
							Node::Info{
								.name = "Spirit Steed's Stride Elevation",
								.type = Utils::EntryType::multiplier,
								.formula = c6Buff,
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
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.5055, 0.5467, 0.5878, 0.6466, 0.6878, 0.7348, 0.7995, 0.8641, 0.9288, 0.9993, 1.0699, 1.1404, 1.2109, 1.2815, 1.3520}),
					},
					Node::Atk{
						.name = "2-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4655, 0.5034, 0.5413, 0.5954, 0.6333, 0.6766, 0.7362, 0.7957, 0.8553, 0.9202, 0.9852, 1.0501, 1.1151, 1.1801, 1.2450}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.3089, 0.3340, 0.3592, 0.3951, 0.4202, 0.4490, 0.4885, 0.5280, 0.5675, 0.6106, 0.6537, 0.6968, 0.7399, 0.7830, 0.8261}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.3089, 0.3340, 0.3592, 0.3951, 0.4202, 0.4490, 0.4885, 0.5280, 0.5675, 0.6106, 0.6537, 0.6968, 0.7399, 0.7830, 0.8261}),
					},
					Node::Atk{
						.name = "4-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.7790, 0.8424, 0.9058, 0.9963, 1.0597, 1.1322, 1.2318, 1.3315, 1.4311, 1.5398, 1.6485, 1.7572, 1.8659, 1.9746, 2.0832}),
					},
				},
				.charged{
					Node::Atk{
						.name = "Charged Attack DMG (x2)",
						.source = Misc::AttackSource::charged,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.7366, 0.7965, 0.8565, 0.9422, 1.0021, 1.0706, 1.1648, 1.2591, 1.3533, 1.4561, 1.5588, 1.6616, 1.7644, 1.8672, 1.9700}),
					},
					Node::Info{
						.name = "Charged Attack Stamina Cost",
						.type = Utils::EntryType::points,
						.formula = Multiplier(Utils::EntryType::points, LevelableTalent::normal, {20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000}),
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
						.name = "Lunar Phase Shift 1-Hit DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.def, LevelableTalent::skill, {0.5658, 0.6082, 0.6507, 0.7072, 0.7497, 0.7921, 0.8487, 0.9053, 0.9618, 1.0184, 1.0750, 1.1316, 1.2023, 1.2730, 1.3438}),
					},
					Node::Atk{
						.name = "Lunar Phase Shift Enhanced 2-Hit DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.def, LevelableTalent::skill, {0.5210, 0.5601, 0.5992, 0.6513, 0.6903, 0.7294, 0.7815, 0.8336, 0.8857, 0.9378, 0.9899, 1.0420, 1.1071, 1.1723, 1.2374}),
					},
					Node::Atk{
						.name = "Lunar Phase Shift 3-Hit DMG (x2)",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.def, LevelableTalent::skill, {0.3457, 0.3716, 0.3975, 0.4321, 0.4580, 0.4840, 0.5185, 0.5531, 0.5877, 0.6222, 0.6568, 0.6914, 0.7346, 0.7778, 0.8210}),
					},
					Node::Atk{
						.name = "Lunar Phase Shift 4-Hit DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.def, LevelableTalent::skill, {0.8718, 0.9372, 1.0026, 1.0897, 1.1551, 1.2205, 1.3077, 1.3949, 1.4820, 1.5692, 1.6564, 1.7436, 1.8525, 1.9615, 2.0705}),
					},
					Node::Atk{
						.name = "Lunar Phase Shift Charged Attack DMG (x2)",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.def, LevelableTalent::skill, {0.6595, 0.7090, 0.7584, 0.8244, 0.8738, 0.9233, 0.9892, 1.0552, 1.1212, 1.1871, 1.2530, 1.3190, 1.4014, 1.4839, 1.5663}),
					},
					Node::Atk{
						.name = "Spirit Steed's Stride 1-Hit DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.def, LevelableTalent::skill, {1.7253, 1.8547, 1.9841, 2.1566, 2.2860, 2.4154, 2.5879, 2.7604, 2.9330, 3.1055, 3.2780, 3.4506, 3.6662, 3.8819, 4.0975})
								 + a1Buff,
						.modifier{
							.elevation = c6Buff,
						},
					},
					Node::DirectLunar{
						.name = "Spirit Steed's Stride 2-Hit DMG",
						.damageType = Misc::LunarDamageType::lunarCrystallize,
						.formula = Multiplier(total.def, LevelableTalent::skill, {1.4097, 1.5154, 1.6211, 1.7621, 1.8678, 1.9736, 2.1145, 2.2555, 2.3965, 2.5374, 2.6784, 2.8194, 2.9956, 3.1718, 3.3480})
								 + a1Buff + c4Buff,
						.modifier{
							.DMG = c1Buff,
							.elevation = c6Buff,
						},
					},
					Node::DirectLunar{
						.name = "Lunar Phase Shift 4-Hit Ascendant Gleam DMG",
						.damageType = Misc::LunarDamageType::lunarCrystallize,
						.formula = Multiplier(total.def, LevelableTalent::skill, {0.2946, 0.3167, 0.3387, 0.3682, 0.3903, 0.4124, 0.4418, 0.4713, 0.5008, 0.5302, 0.5597, 0.5891, 0.6259, 0.6628, 0.6996}),
					},
					Node::Info{
						.name = "Lunar Phase Shift Duration",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000}),
					},
					Node::Info{
						.name = "CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000}),
					},
				},
				.burst{
					Node::Atk{
						.name = "Skill 1-Hit DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.def, LevelableTalent::burst, {1.2696, 1.3648, 1.4600, 1.5870, 1.6822, 1.7774, 1.9044, 2.0314, 2.1583, 2.2853, 2.4122, 2.5392, 2.6979, 2.8566, 3.0153}),
					},
					Node::DirectLunar{
						.name = "Skill 2-Hit DMG",
						.damageType = Misc::LunarDamageType::lunarCrystallize,
						.formula = Multiplier(total.def, LevelableTalent::burst, {1.7774, 1.9107, 2.0441, 2.2218, 2.3551, 2.4884, 2.6662, 2.8439, 3.0216, 3.1994, 3.3771, 3.5549, 3.7771, 3.9992, 4.2214}),
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
				.passive2{
					Node::Mods{
						.mods{
							.preMod{
								.def_ = a4DefBuff,
								.em = a4EmBuff,
							},
						},
					},
				},
				.passive3{
					Node::Mods{
						.mods{
							.teamPreMod{
								.lunarCrystallize{
									.multiplicativeDMG = p3Buff,
								},
							},
						},
					},
				},
			},
		};
	},
};
