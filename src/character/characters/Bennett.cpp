#include "Bennett.hpp"

#include "character/setup.hpp"

const Character::Data Character::Datas::bennett{
	.key{10000032},
	.goodKey{"Bennett"},
	.name{"Bennett"},
	.baseStats{
		.baseHp = 1039.442,
		.baseAtk = 16.027,
		.baseDef = 64.664,
		.ascensionStat = Stat::er,
		.rarity = 4,
		.c3Talent = ConstellationTalent::skill,
		.c5Talent = ConstellationTalent::burst,
		.element = Misc::Element::pyro,
		.weaponType = Misc::WeaponType::sword,
		.hpCurve = Curves::CharacterGrow::HP_S4,
		.atkCurve = Curves::CharacterGrow::ATTACK_S4,
		.defCurve = Curves::CharacterGrow::HP_S4,
		.hpUpgrade = {0.000, 776.516, 1328.251, 2063.898, 2615.634, 3167.369, 3719.104},
		.atkUpgrade = {0.000, 11.973, 20.481, 31.824, 40.332, 48.839, 57.346},
		.defUpgrade = {0.000, 48.307, 82.631, 128.396, 162.720, 197.044, 231.367},
		.ascensionStatUpgrade = {0.000, 0.000, 0.067, 0.133, 0.133, 0.200, 0.267},
	},
	.setup = []() -> Data::Setup {
		auto burstAtkBonusMult = Multiplier(Utils::EntryType::multiplier, LevelableTalent::burst, {0.5600, 0.6020, 0.6440, 0.7000, 0.7420, 0.7840, 0.8400, 0.8960, 0.9520, 1.0080, 1.0640, 1.1200, 1.1900, 1.2600, 1.3300});

		auto burst = IsActive("bennettBurst");
		auto burstBuff = Requires(
			burst,
			burstAtkBonusMult * total.baseAtk
				+ Requires(Requirement::constellation1, 0.2f * total.baseAtk)
		);

		auto c2Cond = IsActive("bennettC2");
		auto c2Buff = Requires(c2Cond && Requirement::constellation2, Constant(0.3f));

		auto isCharacterSword = IsActiveCharacterWeaponType{.type = Misc::WeaponType::sword};
		auto isCharacterClaymore = IsActiveCharacterWeaponType{.type = Misc::WeaponType::claymore};
		auto isCharacterPolearm = IsActiveCharacterWeaponType{.type = Misc::WeaponType::polearm};
		auto c6Cond = isCharacterSword || isCharacterClaymore || isCharacterPolearm;
		auto c6Buff = Requires(Requirement::constellation6 && c6Cond, Constant(0.15f));
		auto c6Infusion = Requires(Requirement::constellation6 && c6Cond, Infusion{.element = Misc::Element::pyro});

		return Data::Setup{
			.mods{
				.preMod{
					.er = c2Buff,
				},
				.teamPreMod{
					.atk = burstBuff,
					.pyro{.DMG = c6Buff},
				},
				.teamInfusion = c6Infusion,
			},
			.opts{
				.burst{
					Option::Boolean{
						.key = "bennettBurst",
						.name = "Active character is inside the field",
						.teamBuff = true,
						.mods{
							.teamPreMod{
								.atk = burstBuff,
								.pyro{.DMG = c6Buff},
							},
							.teamInfusion = c6Infusion,
						},
					},
				},
				.constellation2{
					Option::Boolean{
						.key = "bennettC2",
						.name = "Below 70% HP",
						.mods{
							.preMod{
								.er = c2Buff,
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
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4455, 0.4817, 0.5180, 0.5698, 0.6061, 0.6475, 0.7045, 0.7615, 0.8184, 0.8806, 0.9428, 1.0049, 1.0671, 1.1292, 1.1914}),
					},
					Node::Atk{
						.name = "2-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4274, 0.4622, 0.4970, 0.5467, 0.5815, 0.6212, 0.6759, 0.7306, 0.7853, 0.8449, 0.9045, 0.9642, 1.0238, 1.0835, 1.1431}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.5461, 0.5906, 0.6350, 0.6985, 0.7429, 0.7937, 0.8636, 0.9335, 1.0033, 1.0795, 1.1557, 1.2319, 1.3081, 1.3843, 1.4605}),
					},
					Node::Atk{
						.name = "4-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.5968, 0.6454, 0.6940, 0.7634, 0.8120, 0.8675, 0.9438, 1.0202, 1.0965, 1.1798, 1.2631, 1.3464, 1.4296, 1.5129, 1.5962}),
					},
					Node::Atk{
						.name = "5-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.7190, 0.7775, 0.8360, 0.9196, 0.9781, 1.0450, 1.1370, 1.2289, 1.3209, 1.4212, 1.5215, 1.6218, 1.7222, 1.8225, 1.9228}),
					},
					Node::Atk{
						.name = "Charged Attack DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.5590, 0.6045, 0.6500, 0.7150, 0.7605, 0.8125, 0.8840, 0.9555, 1.0270, 1.1050, 1.1830, 1.2610, 1.3390, 1.4170, 1.4950}),
					},
				},
				.charged{
					Node::Atk{
						.name = "Charged Attack DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.6072, 0.6566, 0.7060, 0.7766, 0.8260, 0.8825, 0.9602, 1.0378, 1.1155, 1.2002, 1.2849, 1.3696, 1.4544, 1.5391, 1.6238}),
					},
					Node::Info{
						.name = "Charged Attack Stamina Cost",
						.formula = Multiplier(LevelableTalent::normal, {20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000}),
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
						.name = "Press DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {1.3760, 1.4792, 1.5824, 1.7200, 1.8232, 1.9264, 2.0640, 2.2016, 2.3392, 2.4768, 2.6144, 2.7520, 2.9240, 3.0960, 3.2680}),
					},
					Node::Atk{
						.name = "Charge Level 1 DMG (1)",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.8400, 0.9030, 0.9660, 1.0500, 1.1130, 1.1760, 1.2600, 1.3440, 1.4280, 1.5120, 1.5960, 1.6800, 1.7850, 1.8900, 1.9950}),
					},
					Node::Atk{
						.name = "Charge Level 1 DMG (2)",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.9200, 0.9890, 1.0580, 1.1500, 1.2190, 1.2880, 1.3800, 1.4720, 1.5640, 1.6560, 1.7480, 1.8400, 1.9550, 2.0700, 2.1850}),
					},
					Node::Atk{
						.name = "Charge Level 2 DMG (1)",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.8800, 0.9460, 1.0120, 1.1000, 1.1660, 1.2320, 1.3200, 1.4080, 1.4960, 1.5840, 1.6720, 1.7600, 1.8700, 1.9800, 2.0900}),
					},
					Node::Atk{
						.name = "Charge Level 2 DMG (2)",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.9600, 1.0320, 1.1040, 1.2000, 1.2720, 1.3440, 1.4400, 1.5360, 1.6320, 1.7280, 1.8240, 1.9200, 2.0400, 2.1600, 2.2800}),
					},
					Node::Atk{
						.name = "Explosion DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {1.3200, 1.4190, 1.5180, 1.6500, 1.7490, 1.8480, 1.9800, 2.1120, 2.2440, 2.3760, 2.5080, 2.6400, 2.8050, 2.9700, 3.1350}),
					},
					Node::Info{
						.name = "Press CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000}),
					},
					Node::Info{
						.name = "Charge Level 1 CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {7.5000, 7.5000, 7.5000, 7.5000, 7.5000, 7.5000, 7.5000, 7.5000, 7.5000, 7.5000, 7.5000, 7.5000, 7.5000, 7.5000, 7.5000}),
					},
					Node::Info{
						.name = "Charge Level 2CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000}),
					},
				},
				.burst{
					Node::Atk{
						.name = "Skill DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.atk, LevelableTalent::burst, {2.3280, 2.5026, 2.6772, 2.9100, 3.0846, 3.2592, 3.4920, 3.7248, 3.9576, 4.1904, 4.4232, 4.6560, 4.9470, 5.2380, 5.5290}),
					},
					Node::Heal{
						.name = "Continuous Regeneration Per Sec",
						.formula = Multiplier(total.atk, LevelableTalent::burst, {0.0600, 0.0645, 0.0690, 0.0750, 0.0795, 0.0840, 0.0900, 0.0960, 0.1020, 0.1080, 0.1140, 0.1200, 0.1275, 0.1350, 0.1425})
								 + Multiplier(total.hp, LevelableTalent::burst, {577.3388, 635.0807, 697.6345, 765.0000, 837.1774, 914.1666, 995.9677, 1082.5804, 1174.0051, 1270.2417, 1371.2900, 1477.1501, 1587.8221, 1703.3059, 1823.6016}),
					},
					Node::Info{
						.name = "ATK Bonus Ratio",
						.type = Utils::EntryType::multiplier,
						.formula = burstAtkBonusMult,
					},
					Node::Info{
						.name = "Duration",
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
						.formula = Multiplier(LevelableTalent::burst, {60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000}),
					},
				},
				.constellation4{
					Node::Atk{
						.name = "Additional attack DMG",
						.source = Misc::AttackSource::skill,
						.formula = 1.35f * Multiplier(total.atk, LevelableTalent::skill, {0.9200, 0.9890, 1.0580, 1.1500, 1.2190, 1.2880, 1.3800, 1.4720, 1.5640, 1.6560, 1.7480, 1.8400, 1.9550, 2.0700, 2.1850}),
					},
				},
			},
		};
	},
};
