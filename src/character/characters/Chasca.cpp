#include "Chasca.hpp"

#include "character/setup.hpp"

const Character::Data Character::Datas::chasca{
	.key{10000104},
	.goodKey{"Chasca"},
	.name{"Chasca"},
	.baseStats{
		.baseHp = 762.656,
		.baseAtk = 26.999,
		.baseDef = 47.864,
		.ascensionStat = Stat::cr,
		.rarity = 5,
		.c3Talent = ConstellationTalent::skill,
		.c5Talent = ConstellationTalent::burst,
		.element = Misc::Element::anemo,
		.weaponType = Misc::WeaponType::bow,
		.hpCurve = Curves::CharacterGrow::HP_S5,
		.atkCurve = Curves::CharacterGrow::ATTACK_S5,
		.defCurve = Curves::CharacterGrow::HP_S5,
		.hpUpgrade = {0.000, 653.909, 1118.528, 1738.020, 2202.639, 2667.259, 3131.878},
		.atkUpgrade = {0.000, 23.148, 39.594, 61.524, 77.971, 94.417, 110.864},
		.defUpgrade = {0.000, 41.040, 70.200, 109.080, 138.240, 167.400, 196.560},
		.ascensionStatUpgrade = {0.000, 0.000, 0.048, 0.096, 0.096, 0.144, 0.192},
	},
	.setup = []() -> Data::Setup {
		auto pyroCount = ElementCountOthers{.element = Misc::Element::pyro};
		auto hydroCount = ElementCountOthers{.element = Misc::Element::hydro};
		auto electroCount = ElementCountOthers{.element = Misc::Element::electro};
		auto cryoCount = ElementCountOthers{.element = Misc::Element::cryo};

		auto pyroCond = Requires(pyroCount >= 1, Constant(1.f));
		auto hydroCond = Requires(hydroCount >= 1, Constant(1.f));
		auto electroCond = Requires(electroCount >= 1, Constant(1.f));
		auto cryoCond = Requires(cryoCount >= 1, Constant(1.f));

		auto c2BonusStack = Requires(Requirement::constellation2, ConstantInt(1));

		auto a1Count = Requires(pyroCount >= 1, ConstantInt(1))
					 + Requires(hydroCount >= 1, ConstantInt(1))
					 + Requires(electroCount >= 1, ConstantInt(1))
					 + Requires(cryoCount >= 1, ConstantInt(1));
		auto a1Buff = Requires(
			Requirement::passive1,
			Index(
				Min(
					a1Count + c2BonusStack,
					3
				),
				true,
				std::array{0.f, 0.15f, 0.35f, 0.65f}
			)
		);

		auto c6Cond = IsActive("chascaC6");
		auto c6Buff = Requires(c6Cond && Requirement::constellation6 && Requirement::passive1, Constant(1.2f));

		auto shadowhuntShellDmg = Modifier{
			.critDMG = c6Buff,
		};
		auto shiningShadowhuntShellDmg = Modifier{
			.DMG = a1Buff,
			.critDMG = c6Buff,
		};

		return Data::Setup{
			.mods{},
			.opts{
				.constellation6{
					Option::Boolean{
						.key = "chascaC6",
						.name = "Fatal Rounds state",
						.nodes{
							Node::Info{
								.name = "Shadowhunt Shell Crit DMG",
								.formula = c6Buff,
							},
							Node::Info{
								.name = "Shining Shadowhunt Shell Crit DMG",
								.formula = c6Buff,
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
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4801, 0.5192, 0.5582, 0.6141, 0.6531, 0.6978, 0.7592, 0.8206, 0.8820, 0.9490, 1.0160, 1.0830, 1.1500, 1.2169, 1.2839}),
					},
					Node::Atk{
						.name = "2-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4459, 0.4822, 0.5185, 0.5703, 0.6066, 0.6481, 0.7051, 0.7622, 0.8192, 0.8814, 0.9436, 1.0058, 1.0680, 1.1303, 1.1925}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.2970, 0.3211, 0.3453, 0.3799, 0.4040, 0.4316, 0.4696, 0.5076, 0.5456, 0.5870, 0.6285, 0.6699, 0.7114, 0.7528, 0.7942}),
					},
					Node::Atk{
						.name = "4-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.2547, 0.2754, 0.2961, 0.3257, 0.3465, 0.3702, 0.4027, 0.4353, 0.4679, 0.5034, 0.5390, 0.5745, 0.6100, 0.6456, 0.6811}),
					},
				},
				.charged{
					Node::Atk{
						.name = "Aimed Shot",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4386, 0.4743, 0.5100, 0.5610, 0.5967, 0.6375, 0.6936, 0.7497, 0.8058, 0.8670, 0.9282, 0.9894, 1.0506, 1.1118, 1.1730}),
					},
					Node::Atk{
						.name = "Fully-Charged Aimed Shot",
						.element = Misc::Element::anemo,
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.2400, 1.3330, 1.4260, 1.5500, 1.6430, 1.7360, 1.8600, 1.9840, 2.1080, 2.2320, 2.3560, 2.4800, 2.6350, 2.7900, 2.9450}),
					},
				},
				.plunge{
					Node::Atk{
						.name = "Plunge DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.5683, 0.6145, 0.6608, 0.7269, 0.7731, 0.8260, 0.8987, 0.9714, 1.0441, 1.1234, 1.2027, 1.2820, 1.3612, 1.4405, 1.5198}),
					},
					Node::Atk{
						.name = "Low Plunge DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.1363, 1.2288, 1.3213, 1.4535, 1.5459, 1.6517, 1.7970, 1.9423, 2.0877, 2.2462, 2.4048, 2.5634, 2.7219, 2.8805, 3.0390}),
					},
					Node::Atk{
						.name = "High Plunge DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.4193, 1.5349, 1.6504, 1.8154, 1.9310, 2.0630, 2.2445, 2.4261, 2.6076, 2.8057, 3.0037, 3.2018, 3.3998, 3.5979, 3.7959}),
					},
				},
				.skill{
					Node::Atk{
						.name = "Resonance DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.6000, 0.6450, 0.6900, 0.7500, 0.7950, 0.8400, 0.9000, 0.9600, 1.0200, 1.0800, 1.1400, 1.2000, 1.2750, 1.3500, 1.4250}),
					},
					Node::Atk{
						.name = "Multitarget Fire Tap DMG",
						.element = Misc::Element::anemo,
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.3600, 0.3870, 0.4140, 0.4500, 0.4770, 0.5040, 0.5400, 0.5760, 0.6120, 0.6480, 0.6840, 0.7200, 0.7650, 0.8100, 0.8550}),
					},
					Node::Atk{
						.name = "Shadowhunt Shell DMG",
						.source = Misc::AttackSource::charged,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.4880, 0.5246, 0.5612, 0.6100, 0.6466, 0.6832, 0.7320, 0.7808, 0.8296, 0.8784, 0.9272, 0.9760, 1.0370, 1.0980, 1.1590}),
						.modifier = shadowhuntShellDmg,
					},
					Node::Atk{
						.name = "Pyro Shining Shadowhunt Shell DMG",
						.element = Misc::Element::pyro,
						.source = Misc::AttackSource::charged,
						.formula = pyroCond * Multiplier(total.atk, LevelableTalent::skill, {1.6657, 1.7906, 1.9156, 2.0821, 2.2071, 2.3320, 2.4986, 2.6652, 2.8317, 2.9983, 3.1649, 3.3314, 3.5397, 3.7479, 3.9561}),
						.modifier = shiningShadowhuntShellDmg,
					},
					Node::Atk{
						.name = "Hydro Shining Shadowhunt Shell DMG",
						.element = Misc::Element::hydro,
						.source = Misc::AttackSource::charged,
						.formula = hydroCond * Multiplier(total.atk, LevelableTalent::skill, {1.6657, 1.7906, 1.9156, 2.0821, 2.2071, 2.3320, 2.4986, 2.6652, 2.8317, 2.9983, 3.1649, 3.3314, 3.5397, 3.7479, 3.9561}),
						.modifier = shiningShadowhuntShellDmg,
					},
					Node::Atk{
						.name = "Electro Shining Shadowhunt Shell DMG",
						.element = Misc::Element::electro,
						.source = Misc::AttackSource::charged,
						.formula = electroCond * Multiplier(total.atk, LevelableTalent::skill, {1.6657, 1.7906, 1.9156, 2.0821, 2.2071, 2.3320, 2.4986, 2.6652, 2.8317, 2.9983, 3.1649, 3.3314, 3.5397, 3.7479, 3.9561}),
						.modifier = shiningShadowhuntShellDmg,
					},
					Node::Atk{
						.name = "Cryo Shining Shadowhunt Shell DMG",
						.element = Misc::Element::cryo,
						.source = Misc::AttackSource::charged,
						.formula = cryoCond * Multiplier(total.atk, LevelableTalent::skill, {1.6657, 1.7906, 1.9156, 2.0821, 2.2071, 2.3320, 2.4986, 2.6652, 2.8317, 2.9983, 3.1649, 3.3314, 3.5397, 3.7479, 3.9561}),
						.modifier = shiningShadowhuntShellDmg,
					},
					Node::Info{
						.name = "Nightsoul Point Limit",
						.formula = Multiplier(LevelableTalent::skill, {80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000}),
					},
					Node::Info{
						.name = "CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {6.5000, 6.5000, 6.5000, 6.5000, 6.5000, 6.5000, 6.5000, 6.5000, 6.5000, 6.5000, 6.5000, 6.5000, 6.5000, 6.5000, 6.5000}),
					},
				},
				.burst{
					Node::Atk{
						.name = "Galesplitting Soulseeker Shell DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.atk, LevelableTalent::burst, {0.8800, 0.9460, 1.0120, 1.1000, 1.1660, 1.2320, 1.3200, 1.4080, 1.4960, 1.5840, 1.6720, 1.7600, 1.8700, 1.9800, 2.0900}),
					},
					Node::Atk{
						.name = "Soulseeker Shell DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.atk, LevelableTalent::burst, {1.0340, 1.1116, 1.1891, 1.2925, 1.3700, 1.4476, 1.5510, 1.6544, 1.7578, 1.8612, 1.9646, 2.0680, 2.1972, 2.3265, 2.4558}),
					},
					Node::Atk{
						.name = "Pyro Radiant Soulseeker Shell DMG",
						.element = Misc::Element::pyro,
						.source = Misc::AttackSource::burst,
						.formula = pyroCond * Multiplier(total.atk, LevelableTalent::burst, {2.0680, 2.2231, 2.3782, 2.5850, 2.7401, 2.8952, 3.1020, 3.3088, 3.5156, 3.7224, 3.9292, 4.1360, 4.3945, 4.6530, 4.9115}),
					},
					Node::Atk{
						.name = "Hydro Radiant Soulseeker Shell DMG",
						.element = Misc::Element::hydro,
						.source = Misc::AttackSource::burst,
						.formula = hydroCond * Multiplier(total.atk, LevelableTalent::burst, {2.0680, 2.2231, 2.3782, 2.5850, 2.7401, 2.8952, 3.1020, 3.3088, 3.5156, 3.7224, 3.9292, 4.1360, 4.3945, 4.6530, 4.9115}),
					},
					Node::Atk{
						.name = "Electro Radiant Soulseeker Shell DMG",
						.element = Misc::Element::electro,
						.source = Misc::AttackSource::burst,
						.formula = electroCond * Multiplier(total.atk, LevelableTalent::burst, {2.0680, 2.2231, 2.3782, 2.5850, 2.7401, 2.8952, 3.1020, 3.3088, 3.5156, 3.7224, 3.9292, 4.1360, 4.3945, 4.6530, 4.9115}),
					},
					Node::Atk{
						.name = "Cryo Radiant Soulseeker Shell DMG",
						.element = Misc::Element::cryo,
						.source = Misc::AttackSource::burst,
						.formula = cryoCond * Multiplier(total.atk, LevelableTalent::burst, {2.0680, 2.2231, 2.3782, 2.5850, 2.7401, 2.8952, 3.1020, 3.3088, 3.5156, 3.7224, 3.9292, 4.1360, 4.3945, 4.6530, 4.9115}),
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
					Node::Info{
						.name = "Shining Shadowhunt Shell DMG Multiplier",
						.type = Utils::EntryType::multiplier,
						.formula = Requires(a1Buff != 0.f, 1.f + a1Buff),
					},
				},
				.passive2{
					Node::Atk{
						.name = "Burning Shadowhunt Shot DMG",
						.source = Misc::AttackSource::charged,
						.formula = 1.5f * Multiplier(total.atk, LevelableTalent::skill, {0.4880, 0.5246, 0.5612, 0.6100, 0.6466, 0.6832, 0.7320, 0.7808, 0.8296, 0.8784, 0.9272, 0.9760, 1.0370, 1.0980, 1.1590}),
						.modifier = shadowhuntShellDmg,
					},
					Node::Atk{
						.name = "Pyro Burning Shadowhunt Shot DMG",
						.element = Misc::Element::pyro,
						.source = Misc::AttackSource::charged,
						.formula = pyroCond * 1.5f * Multiplier(total.atk, LevelableTalent::skill, {1.6657, 1.7906, 1.9156, 2.0821, 2.2071, 2.3320, 2.4986, 2.6652, 2.8317, 2.9983, 3.1649, 3.3314, 3.5397, 3.7479, 3.9561}),
						.modifier = shiningShadowhuntShellDmg,
					},
					Node::Atk{
						.name = "Hydro Burning Shadowhunt Shot DMG",
						.element = Misc::Element::hydro,
						.source = Misc::AttackSource::charged,
						.formula = hydroCond * 1.5f * Multiplier(total.atk, LevelableTalent::skill, {1.6657, 1.7906, 1.9156, 2.0821, 2.2071, 2.3320, 2.4986, 2.6652, 2.8317, 2.9983, 3.1649, 3.3314, 3.5397, 3.7479, 3.9561}),
						.modifier = shiningShadowhuntShellDmg,
					},
					Node::Atk{
						.name = "Electro Burning Shadowhunt Shot DMG",
						.element = Misc::Element::electro,
						.source = Misc::AttackSource::charged,
						.formula = electroCond * 1.5f * Multiplier(total.atk, LevelableTalent::skill, {1.6657, 1.7906, 1.9156, 2.0821, 2.2071, 2.3320, 2.4986, 2.6652, 2.8317, 2.9983, 3.1649, 3.3314, 3.5397, 3.7479, 3.9561}),
						.modifier = shiningShadowhuntShellDmg,
					},
					Node::Atk{
						.name = "Cryo Burning Shadowhunt Shot DMG",
						.element = Misc::Element::cryo,
						.source = Misc::AttackSource::charged,
						.formula = cryoCond * 1.5f * Multiplier(total.atk, LevelableTalent::skill, {1.6657, 1.7906, 1.9156, 2.0821, 2.2071, 2.3320, 2.4986, 2.6652, 2.8317, 2.9983, 3.1649, 3.3314, 3.5397, 3.7479, 3.9561}),
						.modifier = shiningShadowhuntShellDmg,
					},
				},
				.constellation2{
					Node::Atk{
						.name = "Pyro DMG",
						.element = Misc::Element::pyro,
						.source = Misc::AttackSource::charged,
						.formula = pyroCond * 4.f * total.atk,
					},
					Node::Atk{
						.name = "Hydro DMG",
						.element = Misc::Element::hydro,
						.source = Misc::AttackSource::charged,
						.formula = hydroCond * 4.f * total.atk,
					},
					Node::Atk{
						.name = "Electro DMG",
						.element = Misc::Element::electro,
						.source = Misc::AttackSource::charged,
						.formula = electroCond * 4.f * total.atk,
					},
					Node::Atk{
						.name = "Cryo DMG",
						.element = Misc::Element::cryo,
						.source = Misc::AttackSource::charged,
						.formula = cryoCond * 4.f * total.atk,
					},
				},
				.constellation4{
					Node::Atk{
						.name = "Pyro DMG",
						.element = Misc::Element::pyro,
						.source = Misc::AttackSource::charged,
						.formula = pyroCond * 4.f * total.atk,
					},
					Node::Atk{
						.name = "Hydro DMG",
						.element = Misc::Element::hydro,
						.source = Misc::AttackSource::charged,
						.formula = hydroCond * 4.f * total.atk,
					},
					Node::Atk{
						.name = "Electro DMG",
						.element = Misc::Element::electro,
						.source = Misc::AttackSource::charged,
						.formula = electroCond * 4.f * total.atk,
					},
					Node::Atk{
						.name = "Cryo DMG",
						.element = Misc::Element::cryo,
						.source = Misc::AttackSource::charged,
						.formula = cryoCond * 4.f * total.atk,
					},
				},
			},
		};
	},
};
