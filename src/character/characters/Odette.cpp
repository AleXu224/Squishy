#include "Odette.hpp"

#include "character/setup.hpp"

const Character::Data Character::Datas::odette{
	.key{10000150},
	.goodKey{"Odette"},
	.name{"Odette"},
	.baseStats{
		.baseHp = 1010.519,
		.baseAtk = 26.068,
		.baseDef = 61.266,
		.ascensionStat = Stat::cd,
		.rarity = 5,
		.c3Talent = ConstellationTalent::skill,
		.c5Talent = ConstellationTalent::burst,
		.element = Misc::Element::cryo,
		.weaponType = Misc::WeaponType::sword,
		.hpCurve = Curves::CharacterGrow::HP_S5,
		.atkCurve = Curves::CharacterGrow::ATTACK_S5,
		.defCurve = Curves::CharacterGrow::HP_S5,
		.hpUpgrade = {0.000, 866.429, 1482.049, 2302.877, 2918.497, 3534.118, 4149.738},
		.atkUpgrade = {0.000, 22.349, 38.229, 59.402, 75.282, 91.162, 107.041},
		.defUpgrade = {0.000, 52.531, 89.856, 139.622, 176.947, 214.272, 251.597},
		.ascensionStatUpgrade = {0.000, 0.000, 0.096, 0.192, 0.192, 0.288, 0.384},
	},
	.setup = []() -> Data::Setup {
		auto burstActive = IsActive("burstActive");
		auto snowSwanDreamDmgBonus = Multiplier(Utils::EntryType::multiplier, LevelableTalent::burst, {0.1400, 0.1800, 0.2200, 0.2600, 0.3000, 0.3400, 0.3800, 0.4200, 0.4600, 0.5000, 0.5400, 0.5800, 0.6200, 0.6600, 0.7000});
		auto burstBuff = Requires{
			.requirement = burstActive,
			.ret = snowSwanDreamDmgBonus,
		};

		auto marvelousSplendorStacks = GetFloat("marvelousSplendorStacks");
		auto a1Buff = Requires{
			.requirement = Requirement::passive1,
			.ret = 0.15f * marvelousSplendorStacks,
		};

		auto a4Buff = Requires{
			.requirement = Requirement::passive2,
			.ret = Min{
				.val1 = (total.atk - ConstantFlat{.value = 1000.f}) / ConstantFlat{.value = 100.f} * 0.015f,
				.val2 = Constant{.value = 0.3f},
			},
		};

		auto p3Buff = Requires{
			.requirement = Requirement::passive3,
			.ret = Min{
				.val1 = total.atk / ConstantFlat{.value = 100.f} * 0.007f,
				.val2 = Constant{.value = 0.14f},
			},
		};

		auto c2Buff = Requires{
			.requirement = Requirement::constellation2,
			.ret = marvelousSplendorStacks * 0.07f,
		};

		auto soloDanceDoubleActive = IsActive("soloDanceDoubleActive");
		auto c2Shred = Requires{
			.requirement = soloDanceDoubleActive && Requirement::constellation2,
			.ret = Constant{.value = -0.2f},
		};
		auto radianceStellarConduct = IsActiveTeam("radianceStellarConduct");
		auto radianceStellarSwirl = IsActiveTeam("radianceStellarSwirl");
		auto c2CryoShred = Requires{
			.requirement = radianceStellarConduct || radianceStellarSwirl,
			.ret = c2Shred,
		};
		auto c2ElementShred = Requires{
			.requirement = radianceStellarConduct,
			.ret = c2Shred,
		};
		auto c2AnemoShred = Requires{
			.requirement = radianceStellarSwirl,
			.ret = c2Shred,
		};

		auto c4BurstTeamBuff = Requires{
			.requirement = Requirement::constellation4,
			.ret = burstBuff * 0.5f,
		};

		auto c6Elevation = Requires{
			.requirement = Requirement::constellation6 && marvelousSplendorStacks > 0.f,
			.ret = Constant{.value = 0.25f},
		};
		auto c6SelfElevation = Requires{
			.requirement = Requirement::constellation6 && marvelousSplendorStacks > 0.f,
			.ret = Constant{.value = 0.20f},
		};

		return Data::Setup{
			.mods{
				.preMod{
					.allStellar{
						.DMG = burstBuff + a4Buff,
						.elevation = c6SelfElevation,
					},
				},
				.teamPreMod{
					.atk_ = c2Buff,
					.allStellar{
						.DMG = a1Buff + c4BurstTeamBuff,
						.multiplicativeDMG = p3Buff,
						.elevation = c6Elevation,
					},
				},
				.enemy{
					.resistance{
						.cryo = c2CryoShred,
						.electro = c2ElementShred,
						.anemo = c2AnemoShred,
					},
				},
			},
			.opts{
				.burst{
					Option::Boolean{
						.key = "burstActive",
						.name = "After casting Elemental Burst",
						.mods{
							.preMod{
								.allStellar{
									.DMG = burstBuff,
								},
							},
							.teamPreMod{
								.allStellar{
									.DMG = c4BurstTeamBuff,
								},
							},
						},
					},
				},
				.passive1{
					Option::ValueSlider{
						.key = "marvelousSplendorStacks",
						.name = "Marvelous Splendor Stacks",
						.teamBuff = true,
						.values = std::views::iota(0) | std::views::take(7) | std::ranges::to<std::vector<float>>(),
						.mods{
							.preMod{
								.allStellar{
									.elevation = c6SelfElevation,
								},
							},
							.teamPreMod{
								.atk_ = c2Buff,
								.allStellar{
									.DMG = a1Buff,
									.elevation = c6Elevation,
								},
							},
						},
					},
				},
				.constellation2{
					Option::Boolean{
						.key = "soloDanceDoubleActive",
						.name = "Solo Dance Double on the Field",
						.teamBuff = true,
						.mods{
							.enemy{
								.resistance{
									.cryo = c2CryoShred,
									.electro = c2ElementShred,
									.anemo = c2AnemoShred,
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
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.5186, 0.5608, 0.6030, 0.6633, 0.7055, 0.7537, 0.8201, 0.8864, 0.9527, 1.0251, 1.0974, 1.1698, 1.2422, 1.3145, 1.3869}),
					},
					Node::Atk{
						.name = "2-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.5151, 0.5570, 0.5990, 0.6589, 0.7008, 0.7487, 0.8146, 0.8805, 0.9464, 1.0182, 1.0901, 1.1620, 1.2339, 1.3057, 1.3776}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.3261, 0.3527, 0.3792, 0.4171, 0.4437, 0.4740, 0.5157, 0.5574, 0.5991, 0.6446, 0.6901, 0.7356, 0.7812, 0.8267, 0.8722})
								 + Multiplier(total.atk, LevelableTalent::normal, {0.3828, 0.4139, 0.4451, 0.4896, 0.5208, 0.5564, 0.6053, 0.6543, 0.7033, 0.7567, 0.8101, 0.8635, 0.9169, 0.9703, 1.0237}),
					},
					Node::Atk{
						.name = "4-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.7457, 0.8064, 0.8671, 0.9538, 1.0145, 1.0839, 1.1793, 1.2747, 1.3701, 1.4741, 1.5782, 1.6822, 1.7863, 1.8903, 1.9944}),
					},
					Node::Atk{
						.name = "5-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.9021, 0.9756, 1.0490, 1.1539, 1.2273, 1.3112, 1.4266, 1.5420, 1.6574, 1.7833, 1.9091, 2.0350, 2.1609, 2.2868, 2.4127}),
					},
				},
				.charged{
					Node::Atk{
						.name = "Charged Attack DMG",
						.source = Misc::AttackSource::charged,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.0741, 1.1616, 1.2490, 1.3739, 1.4613, 1.5612, 1.6986, 1.8360, 1.9734, 2.1233, 2.2732, 2.4231, 2.5729, 2.7228, 2.8727}),
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
						.name = "Skill DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {1.0808, 1.1619, 1.2429, 1.3510, 1.4321, 1.5131, 1.6212, 1.7293, 1.8374, 1.9454, 2.0535, 2.1616, 2.2967, 2.4318, 2.5669}),
					},
					Node::Atk{
						.name = "Coda at Dawn's Tolling DoT",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.9584, 1.0303, 1.1022, 1.1980, 1.2699, 1.3418, 1.4376, 1.5334, 1.6293, 1.7251, 1.8210, 1.9168, 2.0366, 2.1564, 2.2762}),
					},
					Node::DirectStellar{
						.name = "Coda at Dawn's Tolling Stellar-Conduct",
						.damageType = Misc::StellarDamageType::stellarConduct,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {3.0576, 3.2869, 3.5162, 3.8220, 4.0513, 4.2806, 4.5864, 4.8922, 5.1979, 5.5037, 5.8094, 6.1152, 6.4974, 6.8796, 7.2618}),
					},
					Node::DirectStellar{
						.name = "Coda at Dawn's Tolling Stellar Swirl DMG",
						.damageType = Misc::StellarDamageType::stellarSwirl,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {4.5864, 4.9304, 5.2744, 5.7330, 6.0770, 6.4210, 6.8796, 7.3382, 7.7969, 8.2555, 8.7142, 9.1728, 9.7461, 10.3194, 10.8927}),
					},
					Node::Atk{
						.name = "Plume Dance Move DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.4304, 0.4627, 0.4950, 0.5380, 0.5703, 0.6026, 0.6456, 0.6886, 0.7317, 0.7747, 0.8178, 0.8608, 0.9146, 0.9684, 1.0222}),
					},
					Node::DirectStellar{
						.name = "Plume Dance Move Stellar-Conduct",
						.damageType = Misc::StellarDamageType::stellarConduct,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.2702, 0.2905, 0.3108, 0.3378, 0.3581, 0.3783, 0.4054, 0.4324, 0.4594, 0.4864, 0.5135, 0.5405, 0.5743, 0.6080, 0.6418}),
					},
					Node::DirectStellar{
						.name = "Plume Dance Move Stellar Swirl DMG",
						.damageType = Misc::StellarDamageType::stellarSwirl,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.4053, 0.4357, 0.4661, 0.5066, 0.5370, 0.5674, 0.6079, 0.6484, 0.6890, 0.7295, 0.7700, 0.8106, 0.8612, 0.9119, 0.9625}),
					},
					Node::Atk{
						.name = "Wing Dance Move DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.5146, 0.5532, 0.5918, 0.6433, 0.6819, 0.7205, 0.7720, 0.8234, 0.8749, 0.9264, 0.9778, 1.0293, 1.0936, 1.1579, 1.2223}),
					},
					Node::DirectStellar{
						.name = "Wing Dance Move Stellar-Conduct",
						.damageType = Misc::StellarDamageType::stellarConduct,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.3231, 0.3474, 0.3716, 0.4039, 0.4281, 0.4524, 0.4847, 0.5170, 0.5493, 0.5816, 0.6139, 0.6462, 0.6866, 0.7270, 0.7674}),
					},
					Node::DirectStellar{
						.name = "Wing Dance Move Stellar Swirl DMG",
						.damageType = Misc::StellarDamageType::stellarSwirl,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.4846, 0.5210, 0.5573, 0.6058, 0.6421, 0.6785, 0.7270, 0.7754, 0.8239, 0.8724, 0.9208, 0.9693, 1.0299, 1.0904, 1.1510}),
					},
					Node::Info{
						.name = "Coda at Dawn's Tolling CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000}),
					},
					Node::Info{
						.name = "Solo Dance Double Duration",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000}),
					},
					Node::Info{
						.name = "CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000}),
					},
				},
				.burst{
					Node::Atk{
						.name = "Slash DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.atk, LevelableTalent::burst, {1.1018, 1.1844, 1.2670, 1.3772, 1.4598, 1.5425, 1.6526, 1.7628, 1.8730, 1.9832, 2.0933, 2.2035, 2.3412, 2.4790, 2.6167}),
					},
					Node::Atk{
						.name = "Final Slash DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.atk, LevelableTalent::burst, {1.7027, 1.8304, 1.9581, 2.1284, 2.2561, 2.3838, 2.5541, 2.7244, 2.8946, 3.0649, 3.2352, 3.4054, 3.6183, 3.8311, 4.0440}),
					},
					Node::Info{
						.name = "Snow Swan's Dream Stellar Glimmer Reaction DMG Bonus",
						.type = Utils::EntryType::multiplier,
						.formula = snowSwanDreamDmgBonus,
					},
					Node::Info{
						.name = "Snow Swan's Dream Duration",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::burst, {20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000}),
					},
					Node::Info{
						.name = "Solo Dance Double Duration",
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
				.passive2{
					Node::Mods{
						.mods{
							.preMod{
								.allStellar{
									.DMG = a4Buff,
								},
							},
						},
					},
				},
				.passive3{
					Node::Mods{
						.mods{
							.teamPreMod{
								.allStellar{
									.multiplicativeDMG = p3Buff,
								},
							},
						},
					},
				},
				.constellation1{
					Node::DirectStellar{
						.name = "Stellar-Conduct DMG",
						.damageType = Misc::StellarDamageType::stellarConduct,
						.formula = Requires{
							.requirement = Requirement::constellation1,
							.ret = total.atk * 3.f,
						},
					},
					Node::DirectStellar{
						.name = "Stellar Swirl DMG",
						.damageType = Misc::StellarDamageType::stellarSwirl,
						.formula = Requires{
							.requirement = Requirement::constellation1,
							.ret = total.atk * 4.5f,
						},
					},
				},
				.constellation4{
					Node::DirectStellar{
						.name = "Stellar-Conduct DMG",
						.damageType = Misc::StellarDamageType::stellarConduct,
						.formula = Requires{
							.requirement = Requirement::constellation4,
							.ret = total.atk * 0.66f,
						},
					},
					Node::DirectStellar{
						.name = "Stellar Swirl DMG",
						.damageType = Misc::StellarDamageType::stellarSwirl,
						.formula = Requires{
							.requirement = Requirement::constellation4,
							.ret = total.atk * 0.99f,
						},
					},
				},
			},
		};
	},
};
