#include "Columbina.hpp"

#include "character/setup.hpp"

const Character::Data Character::Datas::columbina{
	.key{10000125},
	.goodKey{"Columbina"},
	.name{"Columbina"},
	.baseStats{
		.baseHp = 1143.984,
		.baseAtk = 7.448,
		.baseDef = 40.086,
		.ascensionStat = Stat::cr,
		.rarity = 5,
		.c3Talent = ConstellationTalent::skill,
		.c5Talent = ConstellationTalent::burst,
		.element = Misc::Element::hydro,
		.weaponType = Misc::WeaponType::catalyst,
		.hpCurve = Curves::CharacterGrow::HP_S5,
		.atkCurve = Curves::CharacterGrow::ATTACK_S5,
		.defCurve = Curves::CharacterGrow::HP_S5,
		.hpUpgrade = {0.000, 980.863, 1677.792, 2607.030, 3303.959, 4000.888, 4697.817},
		.atkUpgrade = {0.000, 6.386, 10.923, 16.972, 21.509, 26.046, 30.583},
		.defUpgrade = {0.000, 34.371, 58.792, 91.354, 115.776, 140.197, 164.619},
		.ascensionStatUpgrade = {0.000, 0.000, 0.048, 0.096, 0.096, 0.144, 0.192},
	},
	.setup = []() -> Data::Setup {
		auto burstLunarMultiplier = Multiplier(Utils::EntryType::multiplier, LevelableTalent::burst, {0.1300, 0.1600, 0.1900, 0.2200, 0.2500, 0.2800, 0.3100, 0.3400, 0.3700, 0.4000, 0.4300, 0.4600, 0.4900, 0.5200, 0.5500});

		auto burstCond = IsActive("columbinaBurstActive");
		auto burstBuff = Requires{
			.requirement = burstCond,
			.ret = burstLunarMultiplier,
		};

		auto a1Cond = GetFloat("columbinaA1Stacks");

		auto a1Buff = Requires{
			.requirement = Requirement::passive1,
			.ret = 0.05f * a1Cond,
		};

		auto p3Buff = Min{
			.val1 = total.hp / ConstantFlat(1000.f) * 0.002f,
			.val2 = 0.07f,
		};

		auto c1Elevation = Requires{
			.requirement = Requirement::constellation1,
			.ret = Constant(0.03f),
		};

		auto c2CondLunarCharged = IsActive("columbinaC2LunarBrillianceLunarCharged");
		auto c2CondLunarBloom = IsActive("columbinaC2LunarBrillianceLunarBloom");
		auto c2CondLunarCrystallize = IsActive("columbinaC2LunarBrillianceLunarCrystallize");
		auto c2BuffLunarCharged = Requires{
			.requirement = Requirement::constellation2 && c2CondLunarCharged && Requirement::ascendantGleam,
			.ret = 0.007f * total.hp,
		};
		auto c2BuffLunarBloom = Requires{
			.requirement = Requirement::constellation2 && c2CondLunarBloom && Requirement::ascendantGleam,
			.ret = 0.0035f * total.hp,
		};
		auto c2BuffLunarCrystallize = Requires{
			.requirement = Requirement::constellation2 && c2CondLunarCrystallize && Requirement::ascendantGleam,
			.ret = 0.007f * total.hp,
		};
		auto c2HpBuff = Requires{
			.requirement = Requirement::constellation2 && (c2CondLunarCharged || c2CondLunarBloom || c2CondLunarCrystallize),
			.ret = Constant(0.20f),
		};
		auto c2Elevation = Requires{
			.requirement = Requirement::constellation2,
			.ret = Constant(0.03f),
		};

		auto c3Elevation = Requires{
			.requirement = Requirement::constellation3,
			.ret = Constant(0.03f),
		};

		auto c4CondLunarCharged = IsActive("columbinaC4KuuvahkiInterferenceLunarCharged");
		auto c4CondLunarBloom = IsActive("columbinaC4KuuvahkiInterferenceLunarBloom");
		auto c4CondLunarCrystallize = IsActive("columbinaC4KuuvahkiInterferenceLunarCrystallize");
		auto c4BuffLunarCharged = Requires{
			.requirement = Requirement::constellation4 && c4CondLunarCharged,
			.ret = Constant(0.125f) * total.hp,
		};
		auto c4BuffLunarBloom = Requires{
			.requirement = Requirement::constellation4 && c4CondLunarBloom,
			.ret = Constant(0.025f) * total.hp,
		};
		auto c4BuffLunarCrystallize = Requires{
			.requirement = Requirement::constellation4 && c4CondLunarCrystallize,
			.ret = Constant(0.125f) * total.hp,
		};
		auto c4Elevation = Requires{
			.requirement = Requirement::constellation4,
			.ret = Constant(0.03f),
		};

		auto c5Elevation = Requires{
			.requirement = Requirement::constellation5,
			.ret = Constant(0.03f),
		};

		auto c6CondLunarCharged = IsActive("columbinaC6LunarCharged");
		auto c6CondLunarBloom = IsActive("columbinaC6LunarBloom");
		auto c6CondLunarCrystallize = IsActive("columbinaC6LunarCrystallize");
		auto c6BuffLunarCharged = Requires{
			.requirement = Requirement::constellation6 && c6CondLunarCharged,
			.ret = Constant(0.8f),
		};
		auto c6BuffLunarBloom = Requires{
			.requirement = Requirement::constellation6 && c6CondLunarBloom,
			.ret = Constant(0.8f),
		};
		auto c6BuffLunarCrystallize = Requires{
			.requirement = Requirement::constellation6 && c6CondLunarCrystallize,
			.ret = Constant(0.8f),
		};
		auto c6HydroBuff = Requires{
			.requirement = Requirement::constellation6 && (c6CondLunarCharged || c6CondLunarBloom || c6CondLunarCrystallize),
			.ret = Constant(0.8f),
		};
		auto c6Elevation = Requires{
			.requirement = Requirement::constellation6,
			.ret = Constant(0.05f),
		};

		return Data::Setup{
			.mods{
				.preMod{
					.hp_ = c2HpBuff,
					.cr = a1Buff,
				},
				.teamPreMod{
					.atk = c2BuffLunarCharged,
					.def = c2BuffLunarCrystallize,
					.em = c2BuffLunarBloom,
					.hydro{.critDMG = c6HydroBuff},
					.electro{.critDMG = c6BuffLunarCharged},
					.dendro{.critDMG = c6BuffLunarBloom},
					.geo{.critDMG = c6BuffLunarCrystallize},
					.allLunar{
						.DMG = burstBuff,
						.multiplicativeDMG = p3Buff,
						.elevation = c1Elevation + c2Elevation + c3Elevation + c4Elevation + c5Elevation + c6Elevation,
					},
				},
				.moonsignLevel = ConstantInt(1),
			},
			.opts{
				.burst{
					Option::Boolean{
						.key = "columbinaBurstActive",
						.name = "Character is within the Lunar Domain",
						.teamBuff = true,
						.mods{
							.teamPreMod{
								.allLunar{.DMG = burstBuff},
							},
						},
					},
				},
				.passive1{
					Option::ValueList{
						.key = "columbinaA1Stacks",
						.prefix = "After triggering Kuuvahki Interference",
						.values{1, 2, 3},
						.mods{
							.preMod{
								.cr = a1Buff,
							},
						},
					},
				},
				.constellation2{
					Option::Boolean{
						.key = "columbinaC2LunarBrillianceLunarCharged",
						.name = "Lunar-Charged Kuuvahki Interference",
						.teamBuff = true,
						.mods{
							.teamPreMod{
								.atk = c2BuffLunarCharged,
							},
						},
					},
					Option::Boolean{
						.key = "columbinaC2LunarBrillianceLunarBloom",
						.name = "Lunar-Bloom Kuuvahki Interference",
						.teamBuff = true,
						.mods{
							.teamPreMod{
								.em = c2BuffLunarBloom,
							},
						},
					},
					Option::Boolean{
						.key = "columbinaC2LunarBrillianceLunarCrystallize",
						.name = "Lunar-Crystallize Kuuvahki Interference",
						.teamBuff = true,
						.mods{
							.teamPreMod{
								.def = c2BuffLunarCrystallize,
							},
						},
					},
				},
				.constellation4{
					Option::Boolean{
						.key = "columbinaC4KuuvahkiInterferenceLunarCharged",
						.name = "Lunar-Charged Kuuvahki Interference",
						.nodes{
							Node::Info{
								.name = "Lunar-Charged Kuuvahki Interference DMG Bonus",
								.type = Utils::EntryType::points,
								.formula = c4BuffLunarCharged,
							},
						},
					},
					Option::Boolean{
						.key = "columbinaC4KuuvahkiInterferenceLunarBloom",
						.name = "Lunar-Bloom Kuuvahki Interference",
						.nodes{
							Node::Info{
								.name = "Lunar-Bloom Kuuvahki Interference DMG Bonus",
								.type = Utils::EntryType::points,
								.formula = c4BuffLunarBloom,
							},
						},
					},
					Option::Boolean{
						.key = "columbinaC4KuuvahkiInterferenceLunarCrystallize",
						.name = "Lunar-Crystallize Kuuvahki Interference",
						.nodes{
							Node::Info{
								.name = "Lunar-Crystallize Kuuvahki Interference DMG Bonus",
								.type = Utils::EntryType::points,
								.formula = c4BuffLunarCrystallize,
							},
						},
					},
				},
				.constellation6{
					Option::Boolean{
						.key = "columbinaC6LunarCharged",
						.name = "After triggering a Lunar-Charged reaction",
						.mods{
							.preMod{
								.electro{.critDMG = c6BuffLunarCharged},
							},
						},
					},
					Option::Boolean{
						.key = "columbinaC6LunarBloom",
						.name = "After triggering a Lunar-Bloom reaction",
						.mods{
							.preMod{
								.dendro{.critDMG = c6BuffLunarBloom},
							},
						},
					},
					Option::Boolean{
						.key = "columbinaC6LunarCrystallize",
						.name = "After triggering a Lunar-Crystallize reaction",
						.mods{
							.preMod{
								.geo{.critDMG = c6BuffLunarCrystallize},
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
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4679, 0.5030, 0.5381, 0.5849, 0.6200, 0.6551, 0.7019, 0.7487, 0.7955, 0.8423, 0.8890, 0.9358, 0.9943, 1.0528, 1.1113}),
					},
					Node::Atk{
						.name = "2-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.3663, 0.3937, 0.4212, 0.4578, 0.4853, 0.5128, 0.5494, 0.5860, 0.6226, 0.6593, 0.6959, 0.7325, 0.7783, 0.8241, 0.8699}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.5848, 0.6287, 0.6726, 0.7310, 0.7749, 0.8188, 0.8773, 0.9357, 0.9942, 1.0527, 1.1112, 1.1697, 1.2428, 1.3159, 1.3890}),
					},
				},
				.charged{
					Node::Atk{
						.name = "Charged Attack DMG",
						.source = Misc::AttackSource::charged,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.1608, 1.2479, 1.3349, 1.4510, 1.5381, 1.6251, 1.7412, 1.8573, 1.9734, 2.0894, 2.2055, 2.3216, 2.4667, 2.6118, 2.7569}),
					},
					Node::Info{
						.name = "Charged Attack Stamina Cost",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::normal, {50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000}),
					},
					Node::DirectLunar{
						.name = "Verdant Dew Charged Attack DMG (x3)",
						.damageType = Misc::LunarDamageType::lunarBloom,
						.formula = Multiplier(total.hp, LevelableTalent::normal, {0.0126, 0.0135, 0.0145, 0.0157, 0.0167, 0.0176, 0.0189, 0.0202, 0.0214, 0.0227, 0.0239, 0.0252, 0.0268, 0.0283, 0.0299}),
					},
				},
				.plunge{
					Node::Atk{
						.name = "Plunge DMG",
						.source = Misc::AttackSource::plunge,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.5683, 0.6145, 0.6608, 0.7269, 0.7731, 0.8260, 0.8987, 0.9714, 1.0441, 1.1234, 1.2027, 1.2820, 1.3612, 1.4405, 1.5198}),
					},
					Node::Atk{
						.name = "Low Plunge DMG",
						.source = Misc::AttackSource::plunge,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.1363, 1.2288, 1.3213, 1.4535, 1.5459, 1.6517, 1.7970, 1.9423, 2.0877, 2.2462, 2.4048, 2.5634, 2.7219, 2.8805, 3.0390}),
					},
					Node::Atk{
						.name = "High Plunge DMG",
						.source = Misc::AttackSource::plunge,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.4193, 1.5349, 1.6504, 1.8154, 1.9310, 2.0630, 2.2445, 2.4261, 2.6076, 2.8057, 3.0037, 3.2018, 3.3998, 3.5979, 3.7959}),
					},
				},
				.skill{
					Node::Atk{
						.name = "Skill DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.hp, LevelableTalent::skill, {0.1672, 0.1797, 0.1923, 0.2090, 0.2215, 0.2341, 0.2508, 0.2675, 0.2842, 0.3010, 0.3177, 0.3344, 0.3553, 0.3762, 0.3971}),
					},
					Node::Atk{
						.name = "Moonreel Continuous DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.hp, LevelableTalent::skill, {0.0936, 0.1006, 0.1076, 0.1170, 0.1240, 0.1310, 0.1404, 0.1498, 0.1591, 0.1685, 0.1778, 0.1872, 0.1989, 0.2106, 0.2223}),
					},
					Node::DirectLunar{
						.name = "Moonreel: Lunar-Charged DMG",
						.damageType = Misc::LunarDamageType::lunarCharged,
						.formula = Multiplier(total.hp, LevelableTalent::skill, {0.0555, 0.0597, 0.0638, 0.0694, 0.0736, 0.0777, 0.0833, 0.0888, 0.0944, 0.0999, 0.1055, 0.1110, 0.1180, 0.1249, 0.1319}),
						.modifier{
							.additiveDMG = c4BuffLunarCharged,
						},
					},
					Node::DirectLunar{
						.name = "Moonreel: Lunar-Bloom DMG (x5)",
						.damageType = Misc::LunarDamageType::lunarBloom,
						.formula = Multiplier(total.hp, LevelableTalent::skill, {0.0200, 0.0215, 0.0230, 0.0250, 0.0265, 0.0280, 0.0300, 0.0320, 0.0340, 0.0360, 0.0380, 0.0400, 0.0425, 0.0450, 0.0475}),
						.modifier{
							.additiveDMG = c4BuffLunarBloom,
						},
					},
					Node::DirectLunar{
						.name = "Moonreel: Lunar-Crystallize DMG",
						.damageType = Misc::LunarDamageType::lunarCrystallize,
						.formula = Multiplier(total.hp, LevelableTalent::skill, {0.1042, 0.1120, 0.1198, 0.1302, 0.1380, 0.1458, 0.1562, 0.1667, 0.1771, 0.1875, 0.1979, 0.2083, 0.2213, 0.2344, 0.2474}),
						.modifier{
							.additiveDMG = c4BuffLunarCrystallize,
						},
					},
					Node::Info{
						.name = "Max Moonreel Charge",
						.type = Utils::EntryType::points,
						.formula = Multiplier(Utils::EntryType::points, LevelableTalent::skill, {60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000}),
					},
					Node::Info{
						.name = "Moonreel Duration",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000}),
					},
					Node::Info{
						.name = "CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {17.0000, 17.0000, 17.0000, 17.0000, 17.0000, 17.0000, 17.0000, 17.0000, 17.0000, 17.0000, 17.0000, 17.0000, 17.0000, 17.0000, 17.0000}),
					},
				},
				.burst{
					Node::Atk{
						.name = "Skill DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.hp, LevelableTalent::burst, {0.3224, 0.3466, 0.3708, 0.4030, 0.4272, 0.4514, 0.4836, 0.5158, 0.5481, 0.5803, 0.6126, 0.6448, 0.6851, 0.7254, 0.7657}),
					},
					Node::Info{
						.name = "Lunar Reaction DMG Bonus",
						.type = Utils::EntryType::multiplier,
						.formula = burstLunarMultiplier,
					},
					Node::Info{
						.name = "Domain Duration",
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
				.passive3{
					Node::Mods{
						.mods{
							.teamPreMod{
								.allLunar{
									.multiplicativeDMG = p3Buff,
								},
							},
						},
					},
				},
				.constellation1{
					Node::Shield{
						.name = "Rainsea Shield",
						.formula = total.hp * 0.12f,
					},
					Node::Shield{
						.name = "Rainsea Shield (Hydro)",
						.element = Misc::Element::hydro,
						.formula = total.hp * 0.12f,
						.modifier = ShieldModifier{
							.elementBonus{2.5f},
						},
					},
					Node::Mods{
						.mods{
							.teamPreMod{
								.allLunar{
									.elevation = c1Elevation,
								},
							},
						},
					},
				},
				.constellation2{
					Node::Mods{
						.mods{
							.preMod{
								.hp_ = c2HpBuff,
							},
							.teamPreMod{
								.allLunar{
									.elevation = c2Elevation,
								},
							},
						},
					},
				},
				.constellation3{
					Node::Mods{
						.mods{
							.teamPreMod{
								.allLunar{
									.elevation = c3Elevation,
								},
							},
						},
					},
				},
				.constellation4{
					Node::Mods{
						.mods{
							.teamPreMod{
								.allLunar{
									.elevation = c4Elevation,
								},
							},
						},
					},
				},
				.constellation5{
					Node::Mods{
						.mods{
							.teamPreMod{
								.allLunar{
									.elevation = c5Elevation,
								},
							},
						},
					},
				},
				.constellation6{
					Node::Mods{
						.mods{
							.preMod{
								.hydro{.critDMG = c6HydroBuff},
							},
							.teamPreMod{
								.allLunar{
									.elevation = c6Elevation,
								},
							},
						},
					},
				},
			},
		};
	},
};
