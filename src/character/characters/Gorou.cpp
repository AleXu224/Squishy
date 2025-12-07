#include "Gorou.hpp"

#include "character/setup.hpp"

const Character::Data Character::Datas::gorou{
	.key{10000055},
	.goodKey{"Gorou"},
	.name{"Gorou"},
	.baseStats{
		.baseHp = 802.376,
		.baseAtk = 15.315,
		.baseDef = 54.364,
		.ascensionStat = Stat::geoDmg,
		.rarity = 4,
		.c3Talent = ConstellationTalent::skill,
		.c5Talent = ConstellationTalent::burst,
		.element = Misc::Element::geo,
		.weaponType = Misc::WeaponType::bow,
		.hpCurve = Curves::CharacterGrow::HP_S4,
		.atkCurve = Curves::CharacterGrow::ATTACK_S4,
		.defCurve = Curves::CharacterGrow::HP_S4,
		.hpUpgrade = {0.000, 599.416, 1025.317, 1593.185, 2019.086, 2444.986, 2870.887},
		.atkUpgrade = {0.000, 11.441, 19.571, 30.410, 38.539, 46.668, 54.798},
		.defUpgrade = {0.000, 40.612, 69.469, 107.944, 136.800, 165.656, 194.512},
		.ascensionStatUpgrade = {0.000, 0.000, 0.060, 0.120, 0.120, 0.180, 0.240},
	},
	.setup = []() -> Data::Setup {
		auto burstCond = IsActive("gorouSkillActive");
		auto burstDefIncrease = Requires{
			.requirement = burstCond && ElementCount{.element = Misc::Element::geo} >= 1,
			.ret = Multiplier(Utils::EntryType::points, LevelableTalent::skill, {206.1600, 221.6220, 237.0840, 257.7000, 273.1620, 288.6240, 309.2400, 329.8560, 350.4720, 371.0880, 391.7040, 412.3200, 438.0900, 463.8600, 489.6300}),
		};
		auto burstGeoDMG = Requires{
			.requirement = burstCond && ElementCount{.element = Misc::Element::geo} >= 3,
			.ret = Multiplier(Utils::EntryType::multiplier, LevelableTalent::skill, {0.1500, 0.1500, 0.1500, 0.1500, 0.1500, 0.1500, 0.1500, 0.1500, 0.1500, 0.1500, 0.1500, 0.1500, 0.1500, 0.1500, 0.1500}),
		};

		auto a1Cond = IsActive("gorouBurstActive");
		auto a1Buff = Requires{
			.requirement = Requirement::passive1 && a1Cond,
			.ret = Constant(0.25f),
		};

		auto a4BuffSkillDMG = Requires{
			.requirement = Requirement::passive2,
			.ret = total.def * 1.56f,
		};
		auto a4BuffBurstDMG = Requires{
			.requirement = Requirement::passive2,
			.ret = total.def * 0.156f,
		};

		auto c6BuffStandingFirm = Requires{
			.requirement = Requirement::constellation6 && a1Cond && ElementCount{.element = Misc::Element::geo} == 1,
			.ret = Constant(0.10f),
		};
		auto c6BuffImpregnable = Requires{
			.requirement = Requirement::constellation6 && a1Cond && ElementCount{.element = Misc::Element::geo} == 2,
			.ret = Constant(0.20f),
		};
		auto c6BuffCrunch = Requires{
			.requirement = Requirement::constellation6 && a1Cond && ElementCount{.element = Misc::Element::geo} >= 3,
			.ret = Constant(0.40f),
		};
		auto c6Buff = c6BuffStandingFirm + c6BuffImpregnable + c6BuffCrunch;

		return Data::Setup{
			.mods{
				.teamPreMod{
					.def = burstDefIncrease,
					.geo{
						.DMG = burstGeoDMG + a1Buff,
						.critDMG = c6Buff,
					},
				},
			},
			.opts{
				.skill{
					Option::Boolean{
						.key = "gorouSkillActive",
						.name = "Gorou Skill Active",
						.teamBuff = true,
						.mods{
							.teamPreMod{
								.def = burstDefIncrease,
								.geo{.DMG = burstGeoDMG},
							},
						},
					},
				},
				.burst{
					Option::Boolean{
						.key = "gorouBurstActive",
						.name = "Gorou Burst Active",
						.teamBuff = true,
						.mods{
							.teamPreMod{
								.geo{.DMG = a1Buff},
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
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.3775, 0.4083, 0.4390, 0.4829, 0.5136, 0.5487, 0.5970, 0.6453, 0.6936, 0.7463, 0.7990, 0.8517, 0.9043, 0.9570, 1.0097}),
					},
					Node::Atk{
						.name = "2-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.3715, 0.4018, 0.4320, 0.4752, 0.5054, 0.5400, 0.5875, 0.6350, 0.6826, 0.7344, 0.7862, 0.8381, 0.8899, 0.9418, 0.9936}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4945, 0.5347, 0.5750, 0.6325, 0.6727, 0.7188, 0.7820, 0.8452, 0.9085, 0.9775, 1.0465, 1.1155, 1.1845, 1.2535, 1.3225}),
					},
					Node::Atk{
						.name = "4-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.5900, 0.6380, 0.6860, 0.7546, 0.8026, 0.8575, 0.9330, 1.0084, 1.0839, 1.1662, 1.2485, 1.3308, 1.4132, 1.4955, 1.5778}),
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
						.element = Misc::Element::geo,
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
						.formula = Multiplier(total.atk, LevelableTalent::skill, {1.0720, 1.1524, 1.2328, 1.3400, 1.4204, 1.5008, 1.6080, 1.7152, 1.8224, 1.9296, 2.0368, 2.1440, 2.2780, 2.4120, 2.5460}),
						.modifier{
							.additiveDMG = a4BuffSkillDMG,
						},
					},
					Node::Info{
						.name = "DEF Increase",
						.type = Utils::EntryType::points,
						.formula = burstDefIncrease,
					},
					Node::Info{
						.name = "Geo DMG Bonus",
						.type = Utils::EntryType::multiplier,
						.formula = burstGeoDMG,
					},
					Node::Info{
						.name = "Duration",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000}),
					},
					Node::Info{
						.name = "CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000, 10.0000}),
					},
				},
				.burst{
					Node::Atk{
						.name = "Skill DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.def, LevelableTalent::burst, {0.9822, 1.0558, 1.1295, 1.2277, 1.3014, 1.3750, 1.4732, 1.5715, 1.6697, 1.7679, 1.8661, 1.9643, 2.0871, 2.2099, 2.3326}),
						.modifier{
							.additiveDMG = a4BuffBurstDMG,
						},
					},
					Node::Atk{
						.name = "Crystal Collapse DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.def, LevelableTalent::burst, {0.6130, 0.6590, 0.7049, 0.7662, 0.8122, 0.8582, 0.9195, 0.9808, 1.0421, 1.1034, 1.1647, 1.2260, 1.3026, 1.3793, 1.4559}),
						.modifier{
							.additiveDMG = a4BuffBurstDMG,
						},
					},
					Node::Info{
						.name = "Duration",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::burst, {9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000}),
					},
					Node::Info{
						.name = "CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::burst, {20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000, 20.0000}),
					},
					Node::Info{
						.name = "Energy Cost",
						.type = Utils::EntryType::energy,
						.formula = Multiplier(Utils::EntryType::energy, LevelableTalent::burst, {80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000}),
					},
				},
				.passive2{
					Node::Info{
						.name = "Skill DMG Increase",
						.type = Utils::EntryType::points,
						.formula = a4BuffSkillDMG,
					},
					Node::Info{
						.name = "Burst DMG Increase",
						.type = Utils::EntryType::points,
						.formula = a4BuffBurstDMG,
					},
				},
				.constellation4{
					Node::Heal{
						.name = "Heal",
						.formula = total.def * 0.5f,
					},
				},
				.constellation6{
					Node::Mods{
						.mods{
							.teamPreMod{
								.geo{
									.critDMG = c6Buff,
								},
							},
						},
					},
				},
			},
		};
	},
};
