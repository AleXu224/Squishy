#include "Neuvillette.hpp"

#include "character/setup.hpp"
#include "ranges"

const Character::Data Character::Datas::neuvillette{
	.key{10000087},
	.goodKey{"Neuvillette"},
	.name{"Neuvillette"},
	.baseStats{
		.baseHp = 1143.984,
		.baseAtk = 16.218,
		.baseDef = 44.873,
		.ascensionStat = Stat::cd,
		.rarity = 5,
		.c3Talent = ConstellationTalent::normal,
		.c5Talent = ConstellationTalent::burst,
		.element = Misc::Element::hydro,
		.weaponType = Misc::WeaponType::catalyst,
		.hpCurve = Curves::CharacterGrow::HP_S5,
		.atkCurve = Curves::CharacterGrow::ATTACK_S5,
		.defCurve = Curves::CharacterGrow::HP_S5,
		.hpUpgrade = {0.000, 980.863, 1677.792, 2607.030, 3303.959, 4000.888, 4697.817},
		.atkUpgrade = {0.000, 13.904, 23.784, 36.957, 46.836, 56.716, 66.595},
		.defUpgrade = {0.000, 38.475, 65.813, 102.262, 129.600, 156.938, 184.275},
		.ascensionStatUpgrade = {0.000, 0.000, 0.096, 0.192, 0.192, 0.288, 0.384},
	},
	.setup = []() -> Data::Setup {
		auto a1Stacks = GetInt("neuvilletteA1");
		auto a1Buff = Requires(
			Requirement::passive1,
			Index(
				a1Stacks,
				true,
				std::array{0.f, 0.1f, 0.25f, 0.6f}
			)
		);

		auto a4Cond = IsActive("neuvilletteA4");
		auto a4Buff = Requires(
			Requirement::passive2 && a4Cond,
			0.006f * (GetFloat("neuvilletteA4") - ConstantFlat(30.f))
		);

		auto c2Buff = Requires(Requirement::constellation2, a1Stacks * 0.14f);

		return Data::Setup{
			.mods{
				.preMod{
					.hydro{.DMG = a4Buff},
				},
			},
			.opts{
				.passive1{
					Option::ValueList{
						.key = "neuvilletteA1",
						.prefix = "Hydro reactions triggered",
						.values{1, 2, 3},
						.nodes{
							Node::Info{
								.name = "Equitable Judgment Multiplier",
								.type = Utils::EntryType::multiplier,
								.formula = Requires(IsActive("neuvilletteA1"), 1.f + a1Buff),
							},
							Node::Info{
								.name = "Equitable Judgment Crit DMG",
								.type = Utils::EntryType::multiplier,
								.formula = c2Buff,
							},
						},
					},
				},
				.passive2{
					Option::ValueList{
						.key = "neuvilletteA4",
						.prefix = "Current HP %",
						.values = std::views::iota(1)
								| std::views::take(50 / 5)
								| std::views::transform([](float val) {
									  return val * 5 + 30;
								  })
								| std::ranges::to<std::vector<uint32_t>>(),
						.mods{
							.preMod{
								.hydro{.DMG = a4Buff},
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
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.5458, 0.5867, 0.6276, 0.6822, 0.7231, 0.7641, 0.8187, 0.8732, 0.9278, 0.9824, 1.0370, 1.0915, 1.1598, 1.2280, 1.2962}),
					},
					Node::Atk{
						.name = "2-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4625, 0.4971, 0.5318, 0.5781, 0.6128, 0.6474, 0.6937, 0.7399, 0.7862, 0.8324, 0.8787, 0.9249, 0.9827, 1.0405, 1.0983}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.7234, 0.7776, 0.8319, 0.9042, 0.9585, 1.0127, 1.0851, 1.1574, 1.2297, 1.3021, 1.3744, 1.4468, 1.5372, 1.6276, 1.7180}),
					},
				},
				.charged{
					Node::Atk{
						.name = "Charged Attack DMG",
						.source = Misc::AttackSource::charged,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.3680, 1.4706, 1.5732, 1.7100, 1.8126, 1.9152, 2.0520, 2.1888, 2.3256, 2.4624, 2.5992, 2.7360, 2.9070, 3.0780, 3.2490}),
					},
					Node::Info{
						.name = "Charged Attack Stamina Cost",
						.formula = Multiplier(LevelableTalent::normal, {50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000}),
					},
					Node::Atk{
						.name = "Charged Attack: Equitable Judgment",
						.source = Misc::AttackSource::charged,
						.formula = Multiplier(total.hp, LevelableTalent::normal, {0.0732, 0.0791, 0.0851, 0.0936, 0.0996, 0.1064, 0.1157, 0.1251, 0.1345, 0.1447, 0.1549, 0.1651, 0.1753, 0.1855, 0.1957}),
						.modifier{
							.multiplicativeDMG = a1Buff,
							.critDMG = c2Buff,
						},
					},
					Node::Info{
						.name = "HP Restored/Droplet",
						.formula = Multiplier(total.hp, LevelableTalent::normal, {0.1600, 0.1600, 0.1600, 0.1600, 0.1600, 0.1600, 0.1600, 0.1600, 0.1600, 0.1600, 0.1600, 0.1600, 0.1600, 0.1600, 0.1600}),
					},
					Node::Info{
						.name = "HP Loss",
						.formula = Multiplier(total.hp, LevelableTalent::normal, {0.0800, 0.0800, 0.0800, 0.0800, 0.0800, 0.0800, 0.0800, 0.0800, 0.0800, 0.0800, 0.0800, 0.0800, 0.0800, 0.0800, 0.0800}),
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
						.formula = Multiplier(total.hp, LevelableTalent::skill, {0.1286, 0.1383, 0.1479, 0.1608, 0.1704, 0.1801, 0.1930, 0.2058, 0.2187, 0.2316, 0.2444, 0.2573, 0.2734, 0.2894, 0.3055}),
					},
					Node::Atk{
						.name = "Spiritbreath Thorn DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.2080, 0.2236, 0.2392, 0.2600, 0.2756, 0.2912, 0.3120, 0.3328, 0.3536, 0.3744, 0.3952, 0.4160, 0.4420, 0.4680, 0.4940}),
					},
					Node::Info{
						.name = "Spiritbreath Thorn Interval",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000}),
					},
					Node::Info{
						.name = "Sourcewater Droplet Duration",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000}),
					},
					Node::Info{
						.name = "CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000}),
					},
				},
				.burst{
					Node::Atk{
						.name = "Skill DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.hp, LevelableTalent::burst, {0.2226, 0.2393, 0.2560, 0.2782, 0.2949, 0.3116, 0.3339, 0.3561, 0.3784, 0.4006, 0.4229, 0.4452, 0.4730, 0.5008, 0.5286}),
					},
					Node::Atk{
						.name = "Waterfall DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.hp, LevelableTalent::burst, {0.0911, 0.0979, 0.1047, 0.1138, 0.1206, 0.1275, 0.1366, 0.1457, 0.1548, 0.1639, 0.1730, 0.1821, 0.1935, 0.2049, 0.2163}),
					},
					Node::Info{
						.name = "CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::burst, {18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000}),
					},
					Node::Info{
						.name = "Energy Cost",
						.formula = Multiplier(LevelableTalent::burst, {70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000, 70.0000}),
					},
				},
				.constellation6{
					Node::Atk{
						.name = "Additional DMG",
						.source = Misc::AttackSource::charged,
						.formula = 0.1f * total.hp,
						.modifier{
							.multiplicativeDMG = a1Buff,
							.critDMG = c2Buff,
						},
					},
				},
			},
		};
	},
};
