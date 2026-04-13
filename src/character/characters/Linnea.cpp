#include "Linnea.hpp"

#include "character/setup.hpp"

const Character::Data Character::Datas::linnea{
	.key{10000130},
	.goodKey{"Linnea"},
	.name{"Linnea"},
	.baseStats{
		.baseHp = 770.283,
		.baseAtk = 11.172,
		.baseDef = 70.599,
		.ascensionStat = Stat::cr,
		.rarity = 5,
		.c3Talent = ConstellationTalent::skill,
		.c5Talent = ConstellationTalent::burst,
		.element = Misc::Element::geo,
		.weaponType = Misc::WeaponType::bow,
		.hpCurve = Curves::CharacterGrow::HP_S5,
		.atkCurve = Curves::CharacterGrow::ATTACK_S5,
		.defCurve = Curves::CharacterGrow::HP_S5,
		.hpUpgrade = {0.000, 660.448, 1129.713, 1755.400, 2224.666, 2693.931, 3163.197},
		.atkUpgrade = {0.000, 9.578, 16.384, 25.458, 32.264, 39.069, 45.875},
		.defUpgrade = {0.000, 60.534, 103.545, 160.893, 203.904, 246.915, 289.926},
		.ascensionStatUpgrade = {0.000, 0.000, 0.048, 0.096, 0.096, 0.144, 0.192},
	},
	.setup = []() -> Data::Setup {
		auto a1Cond = IsActive("linneaA1Cond");
		auto a1ResShred = Requires{
			.requirement = a1Cond && Requirement::passive1,
			.ret = Constant{.value = -0.15f} + Requires{
					   .requirement = Requirement::ascendantGleam,
					   .ret = Constant{.value = -0.15f},
				   },
		};

		auto a4Buff = Requires{.requirement = Requirement::passive2, .ret = total.def * 0.05f};
		auto a4ActiveBuff = Requires{.requirement = ActiveCharacter{.formula = CharacterMoonsignLevel{} >= 1}, .ret = a4Buff};
		auto a4SelfBuff = Requires{.requirement = ActiveCharacter{.formula = CharacterMoonsignLevel{} <= 0}, .ret = a4Buff};

		auto p3Buff = Requires{.requirement = Requirement::passive3, .ret = ConstantInt{.value = 1}};

		auto c1Cond = IsActive("linneaC1");
		auto c6StackMultiplier = IfElse{.requirement = Requirement::constellation6, .trueVal = Constant{.value = 2.f}, .elseVal = Constant{.value = 2.f}};
		auto c1Buff = Requires{
			.requirement = Requirement::constellation1 && c1Cond,
			.ret = total.def * 0.75f * c6StackMultiplier,
		};
		auto c1LumiBuff = Requires{
			.requirement = Requirement::constellation1 && c1Cond,
			.ret = total.def * 1.5f * GetFloat("linneaC1LumiStacks") * c6StackMultiplier,
		};

		auto c2Cond = IsActive("linneaC2MoondriftHarmony");
		auto c2Buff = Requires{
			.requirement = PreviousCharacter{.formula = IsCharacterElement{.element = Misc::Element::geo} || IsCharacterElement{.element = Misc::Element::hydro}}
						&& c2Cond && Requirement::constellation2,
			.ret = Constant{.value = 0.4f},
		};
		auto c2LumiBuff = Requires{.requirement = Requirement::constellation2, .ret = Constant{.value = 1.5f}};

		auto c4Buff = Requires{.requirement = Requirement::constellation4 && c2Cond, .ret = Constant{.value = 0.25f}};

		auto c6Buff = Requires{.requirement = Requirement::constellation6, .ret = Constant{.value = 0.25f}};

		return Data::Setup{
			.mods{
				.preMod{
					.def_ = c4Buff,
					.em = a4SelfBuff,
				},
				.teamPreMod{
					.cd = c2Buff,
					.lunarCrystallize{
						.additiveDMG = c1Buff,
						.elevation = c6Buff,
					},
				},
				.activePreMod{
					.def_ = c4Buff,
					.em = a4ActiveBuff,
				},
				.enemy{
					.resistance{
						.geo = a1ResShred,
					},
				},
				.moonsignLevel = p3Buff,
			},
			.opts{
				.passive1{
					Option::Boolean{
						.key = "linneaA1Cond",
						.name = "Lumi is present on the field",
						.teamBuff = true,
						.mods{
							.enemy{
								.resistance{
									.geo = a1ResShred,
								},
							},
						},
					},
				},
				.constellation1{
					Option::Boolean{
						.key = "linneaC1",
						.name = "Field Catalog active",
						.teamBuff = true,
						.mods{
							.teamPreMod{
								.lunarCrystallize{
									.additiveDMG = c1Buff,
								},
							},
						},
					},
					Option::ValueList{
						.key = "linneaC1LumiStacks",
						.prefix = "Stacks used by Million Ton Crush",
						.displayCondition = IsActive("linneaC1"),
						.values{1, 2, 3, 4, 5},
						.nodes{
							Node::Info{
								.name = "Million Ton Crush DMG Increase",
								.type = Utils::EntryType::points,
								.formula = c1LumiBuff,
							},
						},
					},
				},
				.constellation2{
					Option::Boolean{
						.key = "linneaC2MoondriftHarmony",
						.name = "After triggering Moondrift Harmony",
						.teamBuff = true,
						.mods{
							.preMod{
								.def_ = c4Buff,
							},
							.teamPreMod{
								.cd = c2Buff,
							},
							.activePreMod{
								.def_ = c4Buff,
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
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.5900, 0.6380, 0.6860, 0.7546, 0.8026, 0.8575, 0.9330, 1.0084, 1.0839, 1.1662, 1.2485, 1.3309, 1.4132, 1.4955, 1.5778}),
					},
					Node::Atk{
						.name = "2-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.5115, 0.5532, 0.5948, 0.6543, 0.6959, 0.7435, 0.8089, 0.8743, 0.9398, 1.0111, 1.0825, 1.1539, 1.2253, 1.2966, 1.3680}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.8163, 0.8828, 0.9492, 1.0441, 1.1106, 1.1865, 1.2909, 1.3953, 1.4997, 1.6136, 1.7275, 1.8414, 1.9554, 2.0693, 2.1832}),
					},
				},
				.charged{
					Node::Atk{
						.name = "Aimed Shot",
						.source = Misc::AttackSource::charged,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4386, 0.4743, 0.5100, 0.5610, 0.5967, 0.6375, 0.6936, 0.7497, 0.8058, 0.8670, 0.9282, 0.9894, 1.0506, 1.1118, 1.1730}),
					},
					Node::Atk{
						.name = "Fully-Charged Aimed Shot",
						.source = Misc::AttackSource::charged,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.2400, 1.3330, 1.4260, 1.5500, 1.6430, 1.7360, 1.8600, 1.9840, 2.1080, 2.2320, 2.3560, 2.4800, 2.6350, 2.7900, 2.9450}),
					},
				},
				.plunge{
					Node::Atk{
						.name = "Plunge DMG",
						.source = Misc::AttackSource::plunge,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.5683, 0.6145, 0.6608, 0.7269, 0.7731, 0.8260, 0.8987, 0.9714, 1.0441, 1.1234, 1.2027, 1.2820, 1.3612, 1.4405, 1.5198}),
					},
					Node::Atk{
						.name = "Low/High Plunge DMG",
						.source = Misc::AttackSource::plunge,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.1363, 1.2288, 1.3213, 1.4535, 1.5459, 1.6516, 1.7970, 1.9423, 2.0877, 2.2462, 2.4048, 2.5634, 2.7219, 2.8805, 3.0390}),
					},
					Node::Atk{
						.name = "Low/High Plunge DMG",
						.source = Misc::AttackSource::plunge,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.4193, 1.5349, 1.6504, 1.8154, 1.9310, 2.0630, 2.2445, 2.4261, 2.6076, 2.8057, 3.0037, 3.2018, 3.3998, 3.5979, 3.7959}),
					},
				},
				.skill{
					Node::Atk{
						.name = "Lumi Pound-Pound Pummeler DMG (x2)",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.def, LevelableTalent::skill, {0.9600, 1.0320, 1.1040, 1.2000, 1.2720, 1.3440, 1.4400, 1.5360, 1.6320, 1.7280, 1.8240, 1.9200, 2.0400, 2.1600, 2.2800}),
					},
					Node::DirectLunar{
						.name = "Lumi Heavy Overdrive Hammer DMG",
						.damageType = Misc::LunarDamageType::lunarCrystallize,
						.formula = Multiplier(total.def, LevelableTalent::skill, {1.0000, 1.0750, 1.1500, 1.2500, 1.3250, 1.4000, 1.5000, 1.6000, 1.7000, 1.8000, 1.9000, 2.0000, 2.1250, 2.2500, 2.3750}),
					},
					Node::DirectLunar{
						.name = "Lumi Million Ton Crush DMG",
						.damageType = Misc::LunarDamageType::lunarCrystallize,
						.formula = Multiplier(total.def, LevelableTalent::skill, {4.0000, 4.3000, 4.6000, 5.0000, 5.3000, 5.6000, 6.0000, 6.4000, 6.8000, 7.2000, 7.6000, 8.0000, 8.5000, 9.0000, 9.5000}),
						.modifier{
							.additiveDMG = c1LumiBuff,
							.critDMG = c2LumiBuff,
						},
					},
					Node::Info{
						.name = "Lumi Duration",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000}),
					},
					Node::Info{
						.name = "CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000}),
					},
				},
				.burst{
					Node::Heal{
						.name = "Initial Healing Amount",
						.formula = Multiplier(total.def, LevelableTalent::burst, {1.6000, 1.7200, 1.8400, 2.0000, 2.1200, 2.2400, 2.4000, 2.5600, 2.7200, 2.8800, 3.0400, 3.2000, 3.4000, 3.6000, 3.8000})
								 + Multiplier(Utils::EntryType::points, LevelableTalent::burst, {770.0000, 847.0000, 931.0000, 1021.0000, 1117.0000, 1220.0000, 1329.0000, 1445.0000, 1567.0000, 1695.0000, 1830.0000, 1971.0000, 2119.0000, 2273.0000, 2433.0000}),
					},
					Node::Heal{
						.name = "Continuous Healing",
						.formula = Multiplier(total.def, LevelableTalent::burst, {0.3200, 0.3440, 0.3680, 0.4000, 0.4240, 0.4480, 0.4800, 0.5120, 0.5440, 0.5760, 0.6080, 0.6400, 0.6800, 0.7200, 0.7600})
								 + Multiplier(Utils::EntryType::points, LevelableTalent::burst, {154.0000, 169.0000, 186.0000, 204.0000, 223.0000, 244.0000, 266.0000, 289.0000, 313.0000, 339.0000, 366.0000, 394.0000, 424.0000, 455.0000, 487.0000}),
					},
					Node::Info{
						.name = "Healing Duration",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::burst, {12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000}),
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
								.em = a4SelfBuff,
							},
							.activePreMod{
								.em = a4ActiveBuff,
							},
						},
					},
				},
				.passive3{
					Node::Mods{
						.mods{
							.moonsignLevel = p3Buff,
						},
					},
				},
				.constellation2{
					Node::Info{
						.name = "Million Ton Crush Crit DMG",
						.formula = c2LumiBuff,
					},
				},
				.constellation6{
					Node::Mods{
						.mods{
							.teamPreMod{
								.lunarCrystallize{
									.elevation = c6Buff,
								},
							},
						},
					},
				},
			},
		};
	},
};
