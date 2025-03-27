#include "KamisatoAyaka.hpp"

#include "character/setup.hpp"

const Character::Data Character::Datas::kamisatoAyaka{
	.key{10000002},
	.goodKey{"KamisatoAyaka"},
	.name{"Kamisato Ayaka"},
	.baseStats{
		.baseHp = 1000.986,
		.baseAtk = 26.627,
		.baseDef = 61.027,
		.ascensionStat = Stat::cd,
		.rarity = 5,
		.c3Talent = ConstellationTalent::burst,
		.c5Talent = ConstellationTalent::skill,
		.element = Misc::Element::cryo,
		.weaponType = Misc::WeaponType::sword,
		.hpCurve = Curves::CharacterGrow::HP_S5,
		.atkCurve = Curves::CharacterGrow::ATTACK_S5,
		.defCurve = Curves::CharacterGrow::HP_S5,
		.hpUpgrade = {0.000, 858.255, 1468.068, 2281.151, 2890.964, 3500.777, 4110.590},
		.atkUpgrade = {0.000, 22.828, 39.048, 60.675, 76.895, 93.115, 109.335},
		.defUpgrade = {0.000, 52.326, 89.505, 139.077, 176.256, 213.435, 250.614},
		.ascensionStatUpgrade = {0.000, 0.000, 0.096, 0.192, 0.192, 0.288, 0.384},
	},
	.setup = []() -> Data::Setup {
		auto afterSprinting = IsActive("ayakaSprint");
		auto afterSprintingInfusion = IfElse(afterSprinting, Infusion{Misc::Element::cryo}, NoInfusion{});

		auto a1Cond = IsActive("ayakaA1");
		auto a1Buff = Requires(Requirement::passive1 && a1Cond, Constant(0.3f));

		auto a4Cond = IsActive("ayakaA4");
		auto a4Buff = Requires(Requirement::passive2 && a4Cond, Constant(0.18f));

		auto c4Cond = IsActive("ayakaC4");
		auto c4Def = Requires(Requirement::constellation4 && c4Cond, Constant(0.3f));

		auto c6Cond = IsActive("ayakaC6");
		auto c6Buff = Requires(Requirement::constellation6 && c6Cond, Constant(2.98f));

		return Data::Setup{
			.mods{
				.preMod{
					.cryo{.DMG = a4Buff},
					.normal{.DMG = a1Buff},
					.charged{.DMG = a1Buff + c6Buff},
				},
				.enemy{
					.DEFReduction = c4Def,
				},
				.infusion = afterSprintingInfusion,
			},
			.opts{
				.normal{
					Option::Boolean{
						.key = "ayakaSprint",
						.name = "After sprinting",
						.mods{
							.infusion = afterSprintingInfusion,
						},
					},
				},
				.passive1{
					Option::Boolean{
						.key = "ayakaA1",
						.name = "After using Elemental Skill",
						.mods{
							.preMod{
								.normal{.DMG = a1Buff},
								.charged{.DMG = a1Buff},
							},
						},
					},
				},
				.passive2{
					Option::Boolean{
						.key = "ayakaA4",
						.name = "Sprint hits an opponent",
						.mods{
							.preMod{
								.cryo{.DMG = a4Buff},
							},
						},
					},
				},
				.constellation4{
					Option::Boolean{
						.key = "ayakaC4",
						.name = "Opponent damaged by Elemental Burst",
						.mods{
							.enemy{
								.DEFReduction = c4Def,
							},
						},
					},
				},
				.constellation6{
					Option::Boolean{
						.key = "ayakaC6",
						.name = "Usurahi Butou effect",
						.mods{
							.preMod{
								.charged{.DMG = c6Buff},
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
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4573, 0.4945, 0.5317, 0.5849, 0.6221, 0.6646, 0.7231, 0.7816, 0.8401, 0.9039, 0.9677, 1.0315, 1.0953, 1.1591, 1.2229}),
					},
					Node::Atk{
						.name = "2-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4868, 0.5265, 0.5661, 0.6227, 0.6623, 0.7076, 0.7699, 0.8322, 0.8944, 0.9624, 1.0303, 1.0982, 1.1662, 1.2341, 1.3020}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.6262, 0.6772, 0.7282, 0.8010, 0.8519, 0.9102, 0.9903, 1.0704, 1.1505, 1.2379, 1.3253, 1.4126, 1.5000, 1.5874, 1.6748}),
					},
					Node::Atk{
						.name = "4-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.2265, 0.2449, 0.2633, 0.2897, 0.3081, 0.3292, 0.3581, 0.3871, 0.4161, 0.4477, 0.4793, 0.5109, 0.5425, 0.5741, 0.6057}),
					},
					Node::Atk{
						.name = "5-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.7818, 0.8455, 0.9091, 1.0000, 1.0636, 1.1364, 1.2364, 1.3364, 1.4364, 1.5455, 1.6545, 1.7636, 1.8727, 1.9818, 2.0909}),
					},
				},
				.charged{
					Node::Atk{
						.name = "Charged Attack DMG",
						.source = Misc::AttackSource::charged,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.5513, 0.5961, 0.6410, 0.7051, 0.7500, 0.8012, 0.8718, 0.9423, 1.0128, 1.0897, 1.1666, 1.2435, 1.3205, 1.3974, 1.4743}),
					},
					Node::Info{
						.name = "Charged Attack Stamina Cost",
						.formula = Multiplier(LevelableTalent::normal, {20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000}),
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
						.name = "Skill DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {2.3920, 2.5714, 2.7508, 2.9900, 3.1694, 3.3488, 3.5880, 3.8272, 4.0664, 4.3056, 4.5448, 4.7840, 5.0830, 5.3820, 5.6810}),
					},
					Node::Info{
						.name = "CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000}),
					},
				},
				.burst{
					Node::Atk{
						.name = "Cutting DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.atk, LevelableTalent::burst, {1.1230, 1.2072, 1.2915, 1.4037, 1.4880, 1.5722, 1.6845, 1.7968, 1.9091, 2.0214, 2.1337, 2.2460, 2.3864, 2.5268, 2.6671}),
					},
					Node::Atk{
						.name = "Bloom DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.atk, LevelableTalent::burst, {1.6845, 1.8108, 1.9372, 2.1056, 2.2320, 2.3583, 2.5268, 2.6952, 2.8637, 3.0321, 3.2006, 3.3690, 3.5796, 3.7901, 4.0007}),
					},
					Node::Info{
						.name = "Duration",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::burst, {5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000}),
					},
					Node::Info{
						.name = "CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::burst, {20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000}),
					},
					Node::Info{
						.name = "Energy Cost",
						.formula = Multiplier(LevelableTalent::burst, {80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000}),
					},
				},
				.constellation2{
					Node::Atk{
						.name = "Cutting DMG",
						.source = Misc::AttackSource::burst,
						.formula = 0.2f * Multiplier(total.atk, LevelableTalent::burst, {1.1230, 1.2072, 1.2915, 1.4037, 1.4880, 1.5722, 1.6845, 1.7968, 1.9091, 2.0214, 2.1337, 2.2460, 2.3864, 2.5268, 2.6671}),
					},
					Node::Atk{
						.name = "Bloom DMG",
						.source = Misc::AttackSource::burst,
						.formula = 0.2f * Multiplier(total.atk, LevelableTalent::burst, {1.6845, 1.8108, 1.9372, 2.1056, 2.2320, 2.3583, 2.5268, 2.6952, 2.8637, 3.0321, 3.2006, 3.3690, 3.5796, 3.7901, 4.0007}),
					},
				},
			},
		};
	},
};
