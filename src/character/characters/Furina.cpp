#include "Furina.hpp"

#include "character/setup.hpp"
#include "ranges"

const Character::Data Character::Datas::furina{
	.key{10000089},
	.name{"Furina"},
	.baseStats{
		.baseHp = 1191.650,
		.baseAtk = 18.992,
		.baseDef = 54.146,
		.ascensionStat = Stat::cr,
		.rarity = 5,
		.c3Talent = ConstellationTalent::burst,
		.c5Talent = ConstellationTalent::skill,
		.element = Misc::Element::hydro,
		.weaponType = Misc::WeaponType::sword,
		.hpCurve = Curves::CharacterGrow::HP_S5,
		.atkCurve = Curves::CharacterGrow::ATTACK_S5,
		.defCurve = Curves::CharacterGrow::HP_S5,
		.hpUpgrade = {0.000, 1021.732, 1747.700, 2715.656, 3441.624, 4167.592, 4893.559},
		.atkUpgrade = {0.000, 16.283, 27.853, 43.279, 54.848, 66.418, 77.987},
		.defUpgrade = {0.000, 46.426, 79.414, 123.397, 156.384, 189.371, 222.359},
		.ascensionStatUpgrade = {0.000, 0.000, 0.048, 0.096, 0.096, 0.144, 0.192},
	},
	.opts{
		.burst{
			Option::ValueList{
				.key = "furinaFanfare",
				.prefix = "Fanfare",
				.teamBuff = true,
				.displayCondition = character.constellation == 0,
				.values = std::views::iota(1)
						| std::views::transform([](auto &&val) {
							  return val * 50.f;
						  })
						| std::views::take(6)
						| std::ranges::to<std::vector<uint32_t>>(),
			},
			Option::ValueList{
				.key = "furinaFanfareC1",
				.prefix = "Fanfare",
				.teamBuff = true,
				.displayCondition = Requirement::constellation1,
				.values = std::views::iota(3)
						| std::views::transform([](auto &&val) {
							  return val * 50.f;
						  })
						| std::views::take(6)
						| std::ranges::to<std::vector<uint32_t>>(),
			},
		},
		.constellation2{
			Option::ValueList{
				.key = "furinaAboveFanfareC2",
				.prefix = "Fanfare above limit",
				.displayCondition = GetInt("furinaFanfareC1") > 0,
				.values = std::views::iota(1)
						| std::views::transform([](auto &&val) {
							  return val * 50.f;
						  })
						| std::views::take(8)
						| std::ranges::to<std::vector<uint32_t>>(),
			},
		},
		.constellation6{
			Option::Boolean{
				.key = "furinaCenterOfAttention",
				.name = "Center of Attention active",
			},
			Option::Boolean{
				.key = "furinaC6Pneuma",
				.name = "Pneuma Aligned",
				.displayCondition = IsActive("furinaCenterOfAttention"),
			},
		},
	},
	.setup = []() -> Data::Setup {
		auto fanfareDmgRatio = Multiplier(LevelableTalent::burst, {0.0007, 0.0009, 0.0011, 0.0013, 0.0015, 0.0017, 0.0019, 0.0021, 0.0023, 0.0025, 0.0027, 0.0029, 0.0031, 0.0033, 0.0035});
		auto fanfareIncHealRatio = Multiplier(LevelableTalent::burst, {0.0001, 0.0002, 0.0003, 0.0004, 0.0005, 0.0006, 0.0007, 0.0008, 0.0009, 0.0010, 0.0011, 0.0012, 0.0013, 0.0014, 0.0015});

		auto fanfareStacks = IfElse{
			.requirement = character.constellation == 0,
			.trueVal = GetInt("furinaFanfare"),
			.elseVal = GetInt("furinaFanfareC1"),
		};

		auto a4Points = Requires(
			Requirement::passive2,
			total.hp / ConstantFlat(1000.f)
		);
		auto a4DmgIncrease = Clamp(a4Points * 0.007f, 0.f, 0.28f);

		auto c2FanfareAboveLimit = Requires(
			Requirement::constellation2 && GetInt("furinaFanfareC1") > 0,
			GetFloat("furinaAboveFanfareC2")
		);
		auto c2HpIncrease = Clamp(c2FanfareAboveLimit * Constant(0.0035f), 0.f, 1.4f);

		auto c6Active = IsActive("furinaCenterOfAttention");
		auto c6Pneuma = IsActive("furinaC6Pneuma");
		auto c6Infusion = IfElse{
			Requirement::constellation6 && c6Active,
			Infusion{Misc::Element::hydro},
			NoInfusion{},
		};
		auto c6DmgIncrease = Requires(
			Requirement::constellation6 && c6Active,
			0.18f * total.hp + Requires(c6Pneuma, 0.25f * total.hp)
		);
		auto c6Healing = Requires(
			Requirement::constellation6,
			0.04 * total.hp
		);

		return Data::Setup{
			.mods{
				.preMod{
					.hp_ = c2HpIncrease,
					.normal{.additiveDMG = c6DmgIncrease},
					.charged{.additiveDMG = c6DmgIncrease},
					.plunge{.additiveDMG = c6DmgIncrease},
				},
				.teamPreMod{
					.incHb = fanfareStacks * fanfareIncHealRatio,
					.all{
						.DMG = fanfareStacks * fanfareDmgRatio,
					},
				},
				.infusion = c6Infusion,
			},
			.nodes{
				.normal{
					Node::Atk{
						.name = "1-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4839, 0.5232, 0.5626, 0.6189, 0.6583, 0.7033, 0.7652, 0.8271, 0.8890, 0.9565, 1.0240, 1.0915, 1.1590, 1.2265, 1.2940}),
					},
					Node::Atk{
						.name = "2-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4373, 0.4729, 0.5085, 0.5593, 0.5949, 0.6356, 0.6915, 0.7475, 0.8034, 0.8644, 0.9254, 0.9865, 1.0475, 1.1085, 1.1695}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.5512, 0.5961, 0.6409, 0.7050, 0.7499, 0.8012, 0.8717, 0.9422, 1.0127, 1.0896, 1.1665, 1.2434, 1.3203, 1.3972, 1.4741}),
					},
					Node::Atk{
						.name = "4-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.7330, 0.7926, 0.8523, 0.9375, 0.9972, 1.0654, 1.1591, 1.2529, 1.3466, 1.4489, 1.5512, 1.6535, 1.7557, 1.8580, 1.9603}),
					},
					Node::Atk{
						.name = "Spiritbreath Thorn/Surging Blade DMG",
						.element = Misc::Element::hydro,
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.0946, 0.1023, 0.1100, 0.1210, 0.1287, 0.1375, 0.1496, 0.1617, 0.1738, 0.1870, 0.2002, 0.2134, 0.2266, 0.2398, 0.2530}),
					},
					Node::Info{
						.name = "Spiritbreath Thorn/Surging Blade DMG Interval",
						.formula = Multiplier(LevelableTalent::normal, {6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000}),
					},
				},
				.charged{
					Node::Atk{
						.name = "Charged Attack DMG",
						.source = Misc::AttackSource::charged,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.7422, 0.8026, 0.8630, 0.9493, 1.0097, 1.0788, 1.1737, 1.2686, 1.3635, 1.4671, 1.5707, 1.6742, 1.7778, 1.8813, 1.9849}),
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
						.name = "Ousia Bubble DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.hp, LevelableTalent::skill, {0.0786, 0.0845, 0.0904, 0.0983, 0.1042, 0.1101, 0.1180, 0.1258, 0.1337, 0.1416, 0.1494, 0.1573, 0.1671, 0.1769, 0.1868}),
					},
					Node::Info{
						.name = "Duration",
						.formula = Multiplier(LevelableTalent::skill, {30.0000, 30.0000, 30.0000, 30.0000, 30.0000, 30.0000, 30.0000, 30.0000, 30.0000, 30.0000, 30.0000, 30.0000, 30.0000, 30.0000, 30.0000}),
					},
					Node::Atk{
						.name = "Gentilhomme Usher DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.hp, LevelableTalent::skill, {0.0596, 0.0641, 0.0685, 0.0745, 0.0790, 0.0834, 0.0894, 0.0954, 0.1013, 0.1073, 0.1132, 0.1192, 0.1267, 0.1341, 0.1416}),
						.modifier = Modifier{
							.DMG = a4DmgIncrease,
						},
					},
					Node::Atk{
						.name = "Surintendante Chevalmarin DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.hp, LevelableTalent::skill, {0.0323, 0.0347, 0.0372, 0.0404, 0.0428, 0.0452, 0.0485, 0.0517, 0.0549, 0.0582, 0.0614, 0.0646, 0.0687, 0.0727, 0.0768}),
						.modifier = Modifier{
							.DMG = a4DmgIncrease,
						},
					},
					Node::Atk{
						.name = "Mademoiselle Crabaletta DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.hp, LevelableTalent::skill, {0.0829, 0.0891, 0.0953, 0.1036, 0.1098, 0.1160, 0.1243, 0.1326, 0.1409, 0.1492, 0.1575, 0.1658, 0.1761, 0.1865, 0.1968}),
						.modifier = Modifier{
							.DMG = a4DmgIncrease,
						},
					},
					Node::Info{
						.name = "Gentilhomme Usher HP Consumption",
						.isPercentage = true,
						.formula = Multiplier(LevelableTalent::skill, {0.0240, 0.0240, 0.0240, 0.0240, 0.0240, 0.0240, 0.0240, 0.0240, 0.0240, 0.0240, 0.0240, 0.0240, 0.0240, 0.0240, 0.0240}),
					},
					Node::Info{
						.name = "Surintendante Chevalmarin HP Consumption",
						.isPercentage = true,
						.formula = Multiplier(LevelableTalent::skill, {0.0160, 0.0160, 0.0160, 0.0160, 0.0160, 0.0160, 0.0160, 0.0160, 0.0160, 0.0160, 0.0160, 0.0160, 0.0160, 0.0160, 0.0160}),
					},
					Node::Info{
						.name = "Mademoiselle Crabaletta HP Consumption",
						.isPercentage = true,
						.formula = Multiplier(LevelableTalent::skill, {0.0360, 0.0360, 0.0360, 0.0360, 0.0360, 0.0360, 0.0360, 0.0360, 0.0360, 0.0360, 0.0360, 0.0360, 0.0360, 0.0360, 0.0360}),
					},
					Node::Heal{
						.name = "Singer of Many Waters Healing",
						.formula = Multiplier(total.hp, LevelableTalent::skill, {0.0480, 0.0516, 0.0552, 0.0600, 0.0636, 0.0672, 0.0720, 0.0768, 0.0816, 0.0864, 0.0912, 0.0960, 0.1020, 0.1080, 0.1140}) + Multiplier(LevelableTalent::skill, {462.2253, 508.4543, 558.5356, 612.4694, 670.2556, 731.8942, 797.3852, 866.7286, 939.9245, 1016.9728, 1097.8734, 1182.6265, 1271.2321, 1363.6899, 1460.0002}),
					},
					Node::Info{
						.name = "CD",
						.formula = Multiplier(LevelableTalent::skill, {20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000}),
					},
				},
				.burst{
					Node::Atk{
						.name = "Skill DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.atk, LevelableTalent::burst, {0.1141, 0.1226, 0.1312, 0.1426, 0.1511, 0.1597, 0.1711, 0.1825, 0.1939, 0.2053, 0.2167, 0.2281, 0.2424, 0.2566, 0.2709}),
					},
					Node::Info{
						.name = "Duration",
						.formula = Multiplier(LevelableTalent::burst, {18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000}),
					},
					Node::Info{
						.name = "Maximum Fanfare",
						.formula = Multiplier(LevelableTalent::burst, {300.0000, 300.0000, 300.0000, 300.0000, 300.0000, 300.0000, 300.0000, 300.0000, 300.0000, 300.0000, 300.0000, 300.0000, 300.0000, 300.0000, 300.0000}),
					},
					Node::Info{
						.name = "Fanfare to DMG Increase Conversion Ratio",
						.isPercentage = true,
						.formula = fanfareDmgRatio,
					},
					Node::Info{
						.name = "Fanfare to Incoming Healing Bonus Conversion Ratio",
						.isPercentage = true,
						.formula = fanfareIncHealRatio,
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
					Node::Heal{
						.name = "Endless Waltz healing",
						.formula = total.hp * 0.02f,
					},
				},
				.passive2{
					Node::Info{
						.name = "Salon Members' DMG Increase",
						.isPercentage = true,
						.formula = a4DmgIncrease,
					},
				},
				.constellation2{
					Node::Info{
						.name = "HP Increase",
						.isPercentage = true,
						.formula = c2HpIncrease,
					},
				},
				.constellation6{
					Node::Heal{
						.name = "Ousia Healing",
						.formula = c6Healing,
					},
				},
			},
		};
	},
};
