#include "Escoffier.hpp"

#include "character/setup.hpp"

const Character::Data Character::Datas::escoffier{
	.key{10000112},
	.goodKey{"Escoffier"},
	.name{"Escoffier"},
	.baseStats{
		.baseHp = 1039.119,
		.baseAtk = 26.999,
		.baseDef = 56.958,
		.ascensionStat = Stat::cr,
		.rarity = 5,
		.c3Talent = ConstellationTalent::skill,
		.c5Talent = ConstellationTalent::burst,
		.element = Misc::Element::cryo,
		.weaponType = Misc::WeaponType::polearm,
		.hpCurve = Curves::CharacterGrow::HP_S5,
		.atkCurve = Curves::CharacterGrow::ATTACK_S5,
		.defCurve = Curves::CharacterGrow::HP_S5,
		.hpUpgrade = {0.000, 890.950, 1523.994, 2368.052, 3001.096, 3634.140, 4267.184},
		.atkUpgrade = {0.000, 23.148, 39.594, 61.524, 77.971, 94.417, 110.864},
		.defUpgrade = {0.000, 48.838, 83.538, 129.805, 164.506, 199.206, 233.906},
		.ascensionStatUpgrade = {0.000, 0.000, 0.048, 0.096, 0.096, 0.144, 0.192},
	},
	.setup = []() -> Data::Setup {
		auto hydroCount = ElementCount{Misc::Element::hydro};
		auto cryoCount = ElementCount{Misc::Element::cryo};
		auto isEscoffier = IsActiveCharacterId{10000112};

		auto condA4 = IsActive("escoffierA4");
		auto a4Res = Requires(
			condA4,
			Index{
				.index = hydroCount + cryoCount,
				.isPercentage = true,
				.indexable = std::array{0.f, -0.05f, -0.1f, -0.15f, -0.55f},
			}
		);

		auto condC1 = IsActive("escoffierC1");
		auto c1Buff = Requires(Requirement::constellation1 && Requirement::passive1 && ((hydroCount + cryoCount) >= 4) && condC1, Constant(0.6f));

		auto condC2 = IsActive("escoffierC2");
		auto c2BuffDisplay = Requires(Requirement::constellation2 && condC2, 2.4f * total.atk);
		auto c2Buff = Requires(!isEscoffier, c2BuffDisplay);

		return Data::Setup{
			.mods{
				.teamPreMod{
					.cryo{
						.additiveDMG = c2Buff,
						.critDMG = c1Buff,
					},
				},
				.enemy{
					.resistance{
						.hydro = a4Res,
						.cryo = a4Res,
					},
				},
			},
			.opts{
				.passive2{
					Option::Boolean{
						.key = "escoffierA4",
						.name = "Opponent hit by Elemental Skill or Burst",
						.teamBuff = true,
						.mods{
							.enemy{
								.resistance{
									.hydro = a4Res,
									.cryo = a4Res,
								},
							},
						},
					},
				},
				.constellation1{
					Option::Boolean{
						.key = "escoffierC1",
						.name = "After using Elemental Skill or Burst",
						.teamBuff = true,
						.mods{
							.teamPreMod{
								.cryo{.critDMG = c1Buff},
							},
						},
					},
				},
				.constellation2{
					Option::Boolean{
						.key = "escoffierC2",
						.name = "Stacks active",
						.teamBuff = true,
						.mods{
							.teamPreMod{
								.cryo{
									.additiveDMG = c2Buff,
								},
							},
						},
						.nodes{
							Node::Info{
								.name = "DMG Increase",
								.formula = c2BuffDisplay,
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
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.5155, 0.5575, 0.5994, 0.6594, 0.7013, 0.7493, 0.8152, 0.8812, 0.9471, 1.0190, 1.0910, 1.1629, 1.2348, 1.3068, 1.3787}),
					},
					Node::Atk{
						.name = "2-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4759, 0.5147, 0.5534, 0.6088, 0.6475, 0.6918, 0.7526, 0.8135, 0.8744, 0.9408, 1.0072, 1.0736, 1.1400, 1.2064, 1.2728}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.3300, 0.3569, 0.3837, 0.4221, 0.4489, 0.4796, 0.5219, 0.5641, 0.6063, 0.6523, 0.6984, 0.7444, 0.7905, 0.8365, 0.8825}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4033, 0.4362, 0.4690, 0.5159, 0.5487, 0.5862, 0.6378, 0.6894, 0.7410, 0.7973, 0.8536, 0.9098, 0.9661, 1.0224, 1.0787}),
					},
				},
				.charged{
					Node::Atk{
						.name = "Charged Attack DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.1541, 1.2481, 1.3420, 1.4762, 1.5701, 1.6775, 1.8251, 1.9727, 2.1204, 2.2814, 2.4424, 2.6035, 2.7645, 2.9256, 3.0866}),
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
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.5040, 0.5418, 0.5796, 0.6300, 0.6678, 0.7056, 0.7560, 0.8064, 0.8568, 0.9072, 0.9576, 1.0080, 1.0710, 1.1340, 1.1970}),
					},
					Node::Atk{
						.name = "Frozen Parfait Attack DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {1.2000, 1.2900, 1.3800, 1.5000, 1.5900, 1.6800, 1.8000, 1.9200, 2.0400, 2.1600, 2.2800, 2.4000, 2.5500, 2.7000, 2.8500}),
					},
					Node::Info{
						.name = "Cooking Mek: Cold Storage Mode Duration",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000}),
					},
					Node::Atk{
						.name = "Surging Blade DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.3360, 0.3612, 0.3864, 0.4200, 0.4452, 0.4704, 0.5040, 0.5376, 0.5712, 0.6048, 0.6384, 0.6720, 0.7140, 0.7560, 0.7980}),
					},
					Node::Info{
						.name = "Surging Blade CD Interval",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000}),
					},
					Node::Info{
						.name = "Skill CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000}),
					},
				},
				.burst{
					Node::Atk{
						.name = "Skill DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.atk, LevelableTalent::burst, {5.9280, 6.3726, 6.8172, 7.4100, 7.8546, 8.2992, 8.8920, 9.4848, 10.0776, 10.6704, 11.2632, 11.8560, 12.5970, 13.3380, 14.0790}),
					},
					Node::Heal{
						.name = "Healing",
						.formula = Multiplier(total.atk, LevelableTalent::burst, {1.7203, 1.8493, 1.9784, 2.1504, 2.2794, 2.4084, 2.5805, 2.7525, 2.9245, 3.0966, 3.2686, 3.4406, 3.6557, 3.8707, 4.0858})
								 + Multiplier(LevelableTalent::burst, {1078.5255, 1186.3931, 1303.2495, 1429.0950, 1563.9294, 1707.7528, 1860.5652, 2022.3665, 2193.1567, 2372.9360, 2561.7041, 2759.4614, 2966.2075, 3181.9426, 3406.6665}),
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
				.passive1{
					Node::Heal{
						.name = "Rehab Diet Heal",
						.formula = 1.3824f * total.atk,
					},
				},
				.constellation6{
					Node::Atk{
						.name = "Special-Grade Frozen Parfait DMG",
						.source = Misc::AttackSource::skill,
						.formula = 3.5f * total.atk,
					},
				},
			},
		};
	},
};
