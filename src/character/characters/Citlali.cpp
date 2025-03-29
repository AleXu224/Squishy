#include "Citlali.hpp"

#include "character/setup.hpp"

const Character::Data Character::Datas::citlali{
	.key{10000107},
	.goodKey{"Citlali"},
	.name{"Citlali"},
	.baseStats{
		.baseHp = 905.654,
		.baseAtk = 9.869,
		.baseDef = 59.411,
		.ascensionStat = Stat::em,
		.rarity = 5,
		.c3Talent = ConstellationTalent::skill,
		.c5Talent = ConstellationTalent::burst,
		.element = Misc::Element::cryo,
		.weaponType = Misc::WeaponType::catalyst,
		.hpCurve = Curves::CharacterGrow::HP_S5,
		.atkCurve = Curves::CharacterGrow::ATTACK_S5,
		.defCurve = Curves::CharacterGrow::HP_S5,
		.hpUpgrade = {0.000, 776.516, 1328.252, 2063.899, 2615.634, 3167.370, 3719.105},
		.atkUpgrade = {0.000, 8.461, 14.472, 22.488, 28.500, 34.511, 40.523},
		.defUpgrade = {0.000, 50.941, 87.136, 135.396, 171.590, 207.785, 243.980},
		.ascensionStatUpgrade = {0.000, 0.000, 28.800, 57.600, 57.600, 86.400, 115.200},
	},
	.setup = []() -> Data::Setup {
		auto condA1 = IsActive("citlaliA1");
		auto a1Res = Requires(Requirement::passive1 && condA1, Constant(-0.2f));

		auto a4BuffSkill = Requires(Requirement::passive2, 0.9f * total.em);
		auto a4BuffBurst = Requires(Requirement::passive2, 12.f * total.em);

		auto isCitlali = IsActiveCharacterId{10000107};

		auto condC1 = IsActive("citlaliC1");
		auto c1BuffDisplay = Requires(Requirement::constellation1 && condC1, 2.f * total.em);
		auto c1Buff = Requires(!isCitlali, c1BuffDisplay);

		auto c2EmBuff = Requires(Requirement::constellation2, ConstantFlat(125.f));
		auto condC2 = IsActive("citlaliC2");
		auto c2TeamEmDisplay = Requires(Requirement::constellation2 && condC2, ConstantFlat(250.f));
		auto c2TeamEm = Requires(!isCitlali, c2TeamEmDisplay);
		auto c2Res = Requires(Requirement::constellation2 && Requirement::passive1 && condA1, Constant(-0.2f));

		auto c6Stacks = GetFloat("citlaliC6");
		auto c6TeamBuff = Requires(Requirement::constellation6, c6Stacks * 0.015f);
		auto c6SelfBuff = Requires(Requirement::constellation6, c6Stacks * 0.025f);

		return Data::Setup{
			.mods{
				.preMod{
					.em = c2EmBuff,
					.all{.DMG = c6SelfBuff},
				},
				.teamPreMod{
					.em = c2TeamEm,
					.pyro{.DMG = c6TeamBuff},
					.hydro{.DMG = c6TeamBuff},
					.normal{.additiveDMG = c1Buff},
					.charged{.additiveDMG = c1Buff},
					.plunge{.additiveDMG = c1Buff},
					.skill{.additiveDMG = c1Buff},
					.burst{.additiveDMG = c1Buff},
				},
				.enemy{
					.resistance{
						.pyro = a1Res + c2Res,
						.hydro = a1Res + c2Res,
					},
				},
			},
			.opts{
				.passive1{
					Option::Boolean{
						.key = "citlaliA1",
						.name = "Nearby party members trigger the Frozen or Melt reactions",
						.teamBuff = true,
						.mods{
							.enemy{
								.resistance{
									.pyro = a1Res + c2Res,
									.hydro = a1Res + c2Res,
								},
							},
						},
					},
				},
				.constellation1{
					Option::Boolean{
						.key = "citlaliC1",
						.name = "Opalstar Vestments effect",
						.teamBuff = true,
						.mods{
							.teamPreMod{
								.normal{.additiveDMG = c1Buff},
								.charged{.additiveDMG = c1Buff},
								.plunge{.additiveDMG = c1Buff},
								.skill{.additiveDMG = c1Buff},
								.burst{.additiveDMG = c1Buff},
							},
						},
						.nodes{
							Node::Info{
								.name = "Others DMG Increase",
								.formula = c1BuffDisplay,
							},
						},
					},
				},
				.constellation2{
					Option::Boolean{
						.key = "citlaliC2",
						.name = "Character shielded by Opal Shield",
						.teamBuff = true,
						.mods{
							.teamPreMod{
								.em = c2TeamEm,
							},
						},
						.nodes{
							Node::Info{
								.name = "Others Elemental Mastery",
								.formula = c2TeamEmDisplay,
							},
						},
					},
				},
				.constellation6{
					Option::ValueList{
						.key = "citlaliC6",
						.prefix = "Nightsoul consumed",
						.values{5, 10, 15, 20, 25, 30, 35, 40},
						.mods{
							.preMod{
								.all{.DMG = c6SelfBuff},
							},
							.teamPreMod{
								.pyro{.DMG = c6TeamBuff},
								.hydro{.DMG = c6TeamBuff},
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
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4341, 0.4666, 0.4992, 0.5426, 0.5751, 0.6077, 0.6511, 0.6945, 0.7379, 0.7813, 0.8247, 0.8681, 0.9224, 0.9767, 1.0309}),
					},
					Node::Atk{
						.name = "2-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.3881, 0.4172, 0.4464, 0.4852, 0.5143, 0.5434, 0.5822, 0.6210, 0.6598, 0.6986, 0.7375, 0.7763, 0.8248, 0.8733, 0.9218}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.5377, 0.5780, 0.6184, 0.6721, 0.7125, 0.7528, 0.8066, 0.8603, 0.9141, 0.9679, 1.0217, 1.0754, 1.1426, 1.2099, 1.2771}),
					},
				},
				.charged{
					Node::Atk{
						.name = "Charged Attack DMG",
						.source = Misc::AttackSource::charged,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.9920, 1.0664, 1.1408, 1.2400, 1.3144, 1.3888, 1.4880, 1.5872, 1.6864, 1.7856, 1.8848, 1.9840, 2.1080, 2.2320, 2.3560}),
					},
					Node::Info{
						.name = "Charged Attack Stamina Cost",
						.formula = Multiplier(LevelableTalent::normal, {50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000}),
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
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.1363, 1.2288, 1.3213, 1.4535, 1.5459, 1.6516, 1.7970, 1.9423, 2.0877, 2.2462, 2.4048, 2.5634, 2.7219, 2.8805, 3.0390}),
					},
					Node::Atk{
						.name = "High Plunge DMG",
						.source = Misc::AttackSource::plunge,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.4193, 1.5349, 1.6504, 1.8154, 1.9310, 2.0630, 2.2445, 2.4261, 2.6076, 2.8057, 3.0037, 3.2018, 3.3998, 3.5979, 3.7959}),
					},
				},
				.skill{
					Node::Atk{
						.name = "Obsidian Tzitzimitl DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.7296, 0.7843, 0.8390, 0.9120, 0.9667, 1.0214, 1.0944, 1.1674, 1.2403, 1.3133, 1.3862, 1.4592, 1.5504, 1.6416, 1.7328}),
					},
					Node::Shield{
						.name = "Shield DMG Absorption",
						.formula = Multiplier(total.em, LevelableTalent::skill, {5.7600, 6.1920, 6.6240, 7.2000, 7.6320, 8.0640, 8.6400, 9.2160, 9.7920, 10.3680, 10.9440, 11.5200, 12.2400, 12.9600, 13.6800})
								 + Multiplier(LevelableTalent::skill, {1386.6759, 1525.3628, 1675.6068, 1837.4082, 2010.7667, 2195.6826, 2392.1558, 2600.1860, 2819.7734, 3050.9182, 3293.6204, 3547.8796, 3813.6960, 4091.0698, 4380.0005}),
					},
					Node::Shield{
						.name = "Shield DMG Absorption (cryo)",
						.element = Misc::Element::cryo,
						.formula = Multiplier(total.em, LevelableTalent::skill, {5.7600, 6.1920, 6.6240, 7.2000, 7.6320, 8.0640, 8.6400, 9.2160, 9.7920, 10.3680, 10.9440, 11.5200, 12.2400, 12.9600, 13.6800})
								 + Multiplier(LevelableTalent::skill, {1386.6759, 1525.3628, 1675.6068, 1837.4082, 2010.7667, 2195.6826, 2392.1558, 2600.1860, 2819.7734, 3050.9182, 3293.6204, 3547.8796, 3813.6960, 4091.0698, 4380.0005}),
						.modifier = ShieldModifier{
							.elementBonus{2.5f},
						}
					},
					Node::Info{
						.name = "Shield Duration",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000}),
					},
					Node::Info{
						.name = "Itzpapa Duration",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000}),
					},
					Node::Info{
						.name = "Itzpapa: Opal Fire Nightsoul Point Consumption",
						.formula = Multiplier(LevelableTalent::skill, {8.0000, 8.0000, 8.0000, 8.0000, 8.0000, 8.0000, 8.0000, 8.0000, 8.0000, 8.0000, 8.0000, 8.0000, 8.0000, 8.0000, 8.0000}),
					},
					Node::Atk{
						.name = "Frostfall Storm DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.1702, 0.1830, 0.1958, 0.2128, 0.2256, 0.2383, 0.2554, 0.2724, 0.2894, 0.3064, 0.3235, 0.3405, 0.3618, 0.3830, 0.4043}),
						.modifier = Modifier{
							.additiveDMG = a4BuffSkill,
						},
					},
					Node::Info{
						.name = "Nightsoul Point Limit",
						.formula = Multiplier(LevelableTalent::skill, {100.0000, 100.0000, 100.0000, 100.0000, 100.0000, 100.0000, 100.0000, 100.0000, 100.0000, 100.0000, 100.0000, 100.0000, 100.0000, 100.0000, 100.0000}),
					},
					Node::Info{
						.name = "CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000, 16.0000}),
					},
				},
				.burst{
					Node::Atk{
						.name = "Ice Storm DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.atk, LevelableTalent::burst, {5.3760, 5.7792, 6.1824, 6.7200, 7.1232, 7.5264, 8.0640, 8.6016, 9.1392, 9.6768, 10.2144, 10.7520, 11.4240, 12.0960, 12.7680}),
						.modifier = Modifier{
							.additiveDMG = a4BuffBurst,
						},
					},
					Node::Info{
						.name = "Ice Storm Nightsoul Point Gain",
						.formula = Multiplier(LevelableTalent::burst, {24.0000, 24.0000, 24.0000, 24.0000, 24.0000, 24.0000, 24.0000, 24.0000, 24.0000, 24.0000, 24.0000, 24.0000, 24.0000, 24.0000, 24.0000}),
					},
					Node::Atk{
						.name = "Spiritvessel Skull DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.atk, LevelableTalent::burst, {1.3440, 1.4448, 1.5456, 1.6800, 1.7808, 1.8816, 2.0160, 2.1504, 2.2848, 2.4192, 2.5536, 2.6880, 2.8560, 3.0240, 3.1920}),
					},
					Node::Info{
						.name = "Spiritvessel Skull Nightsoul Point Gain",
						.formula = Multiplier(LevelableTalent::burst, {3.0000, 3.0000, 3.0000, 3.0000, 3.0000, 3.0000, 3.0000, 3.0000, 3.0000, 3.0000, 3.0000, 3.0000, 3.0000, 3.0000, 3.0000}),
					},
					Node::Info{
						.name = "CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::burst, {15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000}),
					},
					Node::Info{
						.name = "Energy Cost",
						.formula = Multiplier(LevelableTalent::burst, {60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000}),
					},
				},
				.passive2{
					Node::Info{
						.name = "Frostfall Storm DMG increase",
						.optimizable = true,
						.formula = a4BuffSkill,
					},
					Node::Info{
						.name = "Ice Storm DMG increase",
						.optimizable = true,
						.formula = a4BuffBurst,
					},
				},
				.constellation4{
					Node::Atk{
						.name = "DMG",
						.source = Misc::AttackSource::burst,
						.formula = 18.f * total.em,
					},
				},
			},
		};
	},
};
