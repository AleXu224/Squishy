#include "Skirk.hpp"

#include "character/setup.hpp"
#include "ranges"

const Character::Data Character::Datas::skirk{
	.key{10000114},
	.goodKey{"Skirk"},
	.name{"Skirk"},
	.baseStats{
		.baseHp = 966.667,
		.baseAtk = 27.930,
		.baseDef = 62.762,
		.ascensionStat = Stat::cd,
		.rarity = 5,
		.c3Talent = ConstellationTalent::burst,
		.c5Talent = ConstellationTalent::skill,
		.element = Misc::Element::cryo,
		.weaponType = Misc::WeaponType::sword,
		.hpCurve = Curves::CharacterGrow::HP_S5,
		.atkCurve = Curves::CharacterGrow::ATTACK_S5,
		.defCurve = Curves::CharacterGrow::HP_S5,
		.hpUpgrade = {0.000, 828.829, 1417.734, 2202.940, 2791.845, 3380.750, 3969.655},
		.atkUpgrade = {0.000, 23.946, 40.960, 63.645, 80.659, 97.673, 114.687},
		.defUpgrade = {0.000, 53.814, 92.050, 143.031, 181.267, 219.503, 257.739},
		.ascensionStatUpgrade = {0.000, 0.000, 0.096, 0.192, 0.192, 0.288, 0.384},
	},
	.setup = []() -> Data::Setup {
		auto serpentsSubtletyBonus = Multiplier(Utils::EntryType::multiplier, LevelableTalent::burst, {0.1663, 0.1787, 0.1912, 0.2078, 0.2203, 0.2328, 0.2494, 0.2660, 0.2826, 0.2993, 0.3159, 0.3325, 0.3533, 0.3741, 0.3949});
		auto burstStacksUsed = IfElse{
			Requirement::constellation2,
			GetFloat("skirkBurstStacksUsedC2"),
			GetFloat("skirkBurstStacksUsed"),
		};
		auto burstBuff = burstStacksUsed * total.atk * serpentsSubtletyBonus;

		auto riftsAbsorbed = IsActive("skirkBurstVoidRifts");
		auto riftsAbsorbedCount = GetInt("skirkBurstVoidRifts");
		auto zeroRiftstBonus = Multiplier(Utils::EntryType::multiplier, LevelableTalent::burst, {0.0350, 0.0400, 0.0450, 0.0500, 0.0550, 0.0600, 0.0650, 0.0700, 0.0750, 0.0800, 0.0850, 0.0900, 0.0950, 0.1000, 0.1050});
		auto oneRiftstBonus = Multiplier(Utils::EntryType::multiplier, LevelableTalent::burst, {0.0660, 0.0720, 0.0780, 0.0840, 0.0900, 0.0960, 0.1020, 0.1080, 0.1140, 0.1200, 0.1260, 0.1320, 0.1380, 0.1440, 0.1500});
		auto twoRiftstBonus = Multiplier(Utils::EntryType::multiplier, LevelableTalent::burst, {0.0880, 0.0960, 0.1040, 0.1120, 0.1200, 0.1280, 0.1360, 0.1440, 0.1520, 0.1600, 0.1680, 0.1760, 0.1840, 0.1920, 0.2000});
		auto threeRiftstBonus = Multiplier(Utils::EntryType::multiplier, LevelableTalent::burst, {0.1100, 0.1200, 0.1300, 0.1400, 0.1500, 0.1600, 0.1700, 0.1800, 0.1900, 0.2000, 0.2100, 0.2200, 0.2300, 0.2400, 0.2500});
		auto burstNaBuff = Requires(
			riftsAbsorbed && Requirement::passive1,
			Evaluator(
				Index(
					riftsAbsorbedCount,
					true,
					std::array{zeroRiftstBonus, oneRiftstBonus, twoRiftstBonus, threeRiftstBonus}
				),
				true
			)
		);

		auto a4Stacks = GetInt("skirkA4Stacks");
		auto a4NormalMult = Index(
			a4Stacks,
			true,
			std::array{0.f, 0.1f, 0.2f, 0.7f}
		);
		auto a4BurstMult = Index(
			a4Stacks,
			true,
			std::array{0.f, 0.05f, 0.15f, 0.6f}
		);

		auto cryoCount = ElementCount{Misc::Element::cryo};
		auto hydroCount = ElementCount{Misc::Element::hydro};
		auto passiveTalentIncrease = Requires(
			(cryoCount + hydroCount) == 4 && cryoCount >= 1 && hydroCount >= 1,
			ConstantInt(1)
		);

		auto c2Cond = IsActive("skirkC2Cond");
		auto c2Buff = Requires(Requirement::constellation2 && c2Cond, Constant(0.6f));

		auto c4AtkBuff = Requires{
			Requirement::constellation4,
			Index{
				a4Stacks,
				true,
				std::array{0.f, 0.1f, 0.2f, 0.5f},
			}
		};

		return Data::Setup{
			.mods{
				.preMod{
					.atk_ = c4AtkBuff,
					.normal{
						.DMG = burstNaBuff + c2Buff,
					},
					.burst{
						.additiveDMG = burstBuff,
					},
				},
				.teamTalents{
					.skill = passiveTalentIncrease,
				},
			},
			.opts{
				.burst{
					Option::ValueList{
						.key = "skirkBurstStacksUsed",
						.prefix = "Serpent's Subtlety points over 50",
						.displayCondition = !Requirement::constellation2,
						.values = std::views::iota(1)
								| std::views::take(12)
								| std::ranges::to<std::vector<uint32_t>>(),
						.mods{
							.preMod{
								.burst{
									.additiveDMG = burstBuff,
								},
							},
						},
					},
					Option::ValueList{
						.key = "skirkBurstStacksUsedC2",
						.prefix = "Serpent's Subtlety points over 50",
						.displayCondition = Requirement::constellation2,
						.values = std::views::iota(1)
								| std::views::take(24)
								| std::ranges::to<std::vector<uint32_t>>(),
						.mods{
							.preMod{
								.burst{
									.additiveDMG = burstBuff,
								},
							},
						},
					},
					Option::ValueList{
						.key = "skirkBurstVoidRifts",
						.prefix = "Rifts absorbed",
						.displayCondition = Requirement::passive1,
						.values{0, 1, 2, 3},
						.mods{
							.preMod{
								.normal{
									.DMG = burstNaBuff,
								},
							},
						},
					},
				},
				.passive2{
					Option::ValueList{
						.key = "skirkA4Stacks",
						.prefix = "Nearby party members deal Hydro or Cryo DMG",
						.values{1, 2, 3},
						.mods{
							.preMod{
								.atk_ = c4AtkBuff,
							},
						},
						.nodes{
							Node::Info{
								.name = "Seven-Phase Flash Normal Attack Mult",
								.type = Utils::EntryType::multiplier,
								.formula = Requires(IsActive("skirkA4Stacks"), 1.f + a4NormalMult),
							},
							Node::Info{
								.name = "Elemental Burst Mult",
								.type = Utils::EntryType::multiplier,
								.formula = Requires(IsActive("skirkA4Stacks"), 1.f + a4BurstMult),
							},
						},
					},
				},
				.constellation2{
					Option::Boolean{
						.key = "skirkC2Cond",
						.name = "In the Seven-Phase Flash mode",
						.mods{
							.preMod{
								.normal{
									.DMG = c2Buff,
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
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.5452, 0.5896, 0.6340, 0.6974, 0.7418, 0.7925, 0.8622, 0.9320, 1.0017, 1.0778, 1.1539, 1.2300, 1.3060, 1.3821, 1.4582}),
					},
					Node::Atk{
						.name = "2-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4979, 0.5385, 0.5790, 0.6369, 0.6774, 0.7238, 0.7874, 0.8511, 0.9148, 0.9843, 1.0538, 1.1233, 1.1927, 1.2622, 1.3317}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.3242, 0.3506, 0.3770, 0.4147, 0.4411, 0.4713, 0.5127, 0.5542, 0.5957, 0.6409, 0.6861, 0.7314, 0.7766, 0.8219, 0.8671}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.3242, 0.3506, 0.3770, 0.4147, 0.4411, 0.4713, 0.5127, 0.5542, 0.5957, 0.6409, 0.6861, 0.7314, 0.7766, 0.8219, 0.8671}),
					},
					Node::Atk{
						.name = "4-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.6080, 0.6575, 0.7070, 0.7777, 0.8272, 0.8838, 0.9615, 1.0393, 1.1171, 1.2019, 1.2867, 1.3716, 1.4564, 1.5413, 1.6261}),
					},
					Node::Atk{
						.name = "5-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.8290, 0.8965, 0.9640, 1.0604, 1.1279, 1.2050, 1.3110, 1.4171, 1.5231, 1.6388, 1.7545, 1.8702, 1.9858, 2.1015, 2.2172}),
					},
				},
				.charged{
					Node::Atk{
						.name = "Charged Attack DMG",
						.source = Misc::AttackSource::charged,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.6682, 0.7226, 0.7770, 0.8547, 0.9091, 0.9712, 1.0567, 1.1422, 1.2277, 1.3209, 1.4141, 1.5074, 1.6006, 1.6939, 1.7871}),
					},
					Node::Atk{
						.name = "Charged Attack Stamina Cost",
						.source = Misc::AttackSource::charged,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000}),
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
						.name = "1-Hit DMG",
						.element = Misc::Element::cryo,
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {1.3282, 1.4364, 1.5445, 1.6989, 1.8070, 1.9306, 2.1005, 2.2704, 2.4403, 2.6256, 2.8109, 2.9963, 3.1816, 3.3669, 3.5523}),
						.modifier{.multiplicativeDMG = a4NormalMult},
					},
					Node::Atk{
						.name = "2-Hit DMG",
						.element = Misc::Element::cryo,
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {1.1980, 1.2955, 1.3930, 1.5323, 1.6298, 1.7413, 1.8945, 2.0477, 2.2010, 2.3681, 2.5353, 2.7025, 2.8696, 3.0368, 3.2039}),
						.modifier{.multiplicativeDMG = a4NormalMult},
					},
					Node::Atk{
						.name = "3-Hit DMG (1)",
						.element = Misc::Element::cryo,
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.7572, 0.8189, 0.8805, 0.9686, 1.0302, 1.1006, 1.1975, 1.2943, 1.3912, 1.4969, 1.6025, 1.7082, 1.8138, 1.9195, 2.0252}),
						.modifier{.multiplicativeDMG = a4NormalMult},
					},
					Node::Atk{
						.name = "3-Hit DMG (2)",
						.element = Misc::Element::cryo,
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.7572, 0.8189, 0.8805, 0.9686, 1.0302, 1.1006, 1.1975, 1.2943, 1.3912, 1.4969, 1.6025, 1.7082, 1.8138, 1.9195, 2.0252}),
						.modifier{.multiplicativeDMG = a4NormalMult},
					},
					Node::Atk{
						.name = "4-Hit DMG (1)",
						.element = Misc::Element::cryo,
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.8054, 0.8709, 0.9365, 1.0301, 1.0957, 1.1706, 1.2736, 1.3767, 1.4797, 1.5920, 1.7044, 1.8168, 1.9292, 2.0416, 2.1540}),
						.modifier{.multiplicativeDMG = a4NormalMult},
					},
					Node::Atk{
						.name = "4-Hit DMG (2)",
						.element = Misc::Element::cryo,
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.8054, 0.8709, 0.9365, 1.0301, 1.0957, 1.1706, 1.2736, 1.3767, 1.4797, 1.5920, 1.7044, 1.8168, 1.9292, 2.0416, 2.1540}),
						.modifier{.multiplicativeDMG = a4NormalMult},
					},
					Node::Atk{
						.name = "5-Hit DMG",
						.element = Misc::Element::cryo,
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {1.9662, 2.1263, 2.2863, 2.5150, 2.6750, 2.8579, 3.1094, 3.3609, 3.6124, 3.8868, 4.1611, 4.4355, 4.7098, 4.9842, 5.2586}),
						.modifier{.multiplicativeDMG = a4NormalMult},
					},
					Node::Atk{
						.name = "Charged Attack DMG",
						.element = Misc::Element::cryo,
						.source = Misc::AttackSource::charged,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.4455, 0.4817, 0.5180, 0.5698, 0.6061, 0.6475, 0.7045, 0.7615, 0.8184, 0.8806, 0.9428, 1.0049, 1.0671, 1.1292, 1.1914}),
					},
					Node::Info{
						.name = "Charged Attack Stamina Cost",
						.formula = Multiplier(LevelableTalent::skill, {25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000}),
					},
					Node::Atk{
						.name = "Plunge DMG",
						.element = Misc::Element::cryo,
						.source = Misc::AttackSource::plunge,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.6393, 0.6914, 0.7434, 0.8177, 0.8698, 0.9293, 1.0110, 1.0928, 1.1746, 1.2638, 1.3530, 1.4422, 1.5314, 1.6206, 1.7098}),
					},
					Node::Atk{
						.name = "Low Plunge DMG",
						.element = Misc::Element::cryo,
						.source = Misc::AttackSource::plunge,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {1.2784, 1.3824, 1.4865, 1.6351, 1.7392, 1.8581, 2.0216, 2.1851, 2.3486, 2.5270, 2.7054, 2.8838, 3.0622, 3.2405, 3.4189}),
					},
					Node::Atk{
						.name = "High Plunge DMG",
						.source = Misc::AttackSource::plunge,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {1.5968, 1.7267, 1.8567, 2.0424, 2.1723, 2.3209, 2.5251, 2.7293, 2.9336, 3.1564, 3.3792, 3.6020, 3.8248, 4.0476, 4.2704}),
					},
					Node::Info{
						.name = "Seven-Phase Flash Mode Duration",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {12.5000, 12.5000, 12.5000, 12.5000, 12.5000, 12.5000, 12.5000, 12.5000, 12.5000, 12.5000, 12.5000, 12.5000, 12.5000, 12.5000, 12.5000}),
					},
					Node::Info{
						.name = "Max Serpent's Subtlety",
						.formula = Multiplier(LevelableTalent::skill, {100.0000, 100.0000, 100.0000, 100.0000, 100.0000, 100.0000, 100.0000, 100.0000, 100.0000, 100.0000, 100.0000, 100.0000, 100.0000, 100.0000, 100.0000}),
					},
					Node::Info{
						.name = "CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000}),
					},
				},
				.burst{
					Node::Atk{
						.name = "Skill DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.atk, LevelableTalent::burst, {1.3812, 1.4848, 1.5884, 1.7265, 1.8301, 1.9337, 2.0718, 2.2099, 2.3480, 2.4862, 2.6243, 2.7624, 2.9350, 3.1077, 3.2803}),
						.modifier{.multiplicativeDMG = a4BurstMult},
					},
					Node::Atk{
						.name = "Skill Final Attack DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.atk, LevelableTalent::burst, {2.3020, 2.4747, 2.6473, 2.8775, 3.0501, 3.2228, 3.4530, 3.6832, 3.9134, 4.1436, 4.3738, 4.6040, 4.8918, 5.1795, 5.4672}),
						.modifier{.multiplicativeDMG = a4BurstMult},
					},
					Node::Info{
						.name = "Serpent's Subtlety Bonus",
						.type = Utils::EntryType::multiplier,
						.formula = serpentsSubtletyBonus,
					},
					Node::Info{
						.name = "0 Void Rift Absorption DMG Bonus",
						.type = Utils::EntryType::multiplier,
						.formula = zeroRiftstBonus,
					},
					Node::Info{
						.name = "1 Void Rift Absorption DMG Bonus",
						.type = Utils::EntryType::multiplier,
						.formula = oneRiftstBonus,
					},
					Node::Info{
						.name = "2 Void Rift Absorption DMG Bonus",
						.type = Utils::EntryType::multiplier,
						.formula = twoRiftstBonus,
					},
					Node::Info{
						.name = "3 Void Rift Absorption DMG Bonus",
						.type = Utils::EntryType::multiplier,
						.formula = threeRiftstBonus,
					},
					Node::Info{
						.name = "CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::burst, {15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000}),
					},
				},
				.constellation1{
					Node::Atk{
						.name = "Crystal Blade DMG",
						.element = Misc::Element::cryo,
						.source = Misc::AttackSource::charged,
						.formula = Requires(Requirement::passive1, total.atk * 5.f),
					},
				},
				.constellation6{
					Node::Atk{
						.name = "Elemental Burst Coordinated Attack",
						.source = Misc::AttackSource::burst,
						.formula = Requires(Requirement::passive1, total.atk * 6.f),
						.modifier{.multiplicativeDMG = a4BurstMult},
					},
					Node::Atk{
						.name = "Normal Attack Coordinated Attack",
						.element = Misc::Element::cryo,
						.source = Misc::AttackSource::normal,
						.formula = Requires(Requirement::passive1, total.atk * 1.5f),
						.modifier{.multiplicativeDMG = a4NormalMult},
					},
					Node::Atk{
						.name = "Normal Attack Coordinated Attack",
						.element = Misc::Element::cryo,
						.source = Misc::AttackSource::charged,
						.formula = Requires(Requirement::passive1, total.atk * 1.5f),
					},
				},
			},
		};
	},
};
