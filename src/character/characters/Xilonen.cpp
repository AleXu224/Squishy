#include "Xilonen.hpp"

#include "character/setup.hpp"

const Character::Data Character::Datas::xilonen{
	.key{10000103},
	.name{"Xilonen"},
	.baseStats{
		.baseHp = 965.713,
		.baseAtk = 21.413,
		.baseDef = 72.394,
		.ascensionStat = Stat::def_,
		.rarity = 5,
		.c3Talent = ConstellationTalent::skill,
		.c5Talent = ConstellationTalent::burst,
		.element = Misc::Element::geo,
		.weaponType = Misc::WeaponType::sword,
		.hpCurve = Curves::CharacterGrow::HP_S5,
		.atkCurve = Curves::CharacterGrow::ATTACK_S5,
		.defCurve = Curves::CharacterGrow::HP_S5,
		.hpUpgrade = {0.000, 828.012, 1416.336, 2200.768, 2789.092, 3377.416, 3965.740},
		.atkUpgrade = {0.000, 18.358, 31.402, 48.795, 61.839, 74.883, 87.927},
		.defUpgrade = {0.000, 62.073, 106.177, 164.984, 209.088, 253.193, 297.297},
		.ascensionStatUpgrade = {0.000, 0.000, 0.090, 0.180, 0.180, 0.270, 0.360},
	},
	.setup = []() -> Data::Setup {
		auto samplesActivated = IsActive("xilonenSamplesActivated");
		auto phecCount = PHECCount{};
		auto resDecreaseMult = 0.f - Multiplier(EntryType::multiplier, LevelableTalent::skill, {0.0900, 0.1200, 0.1500, 0.1800, 0.2100, 0.2400, 0.2700, 0.3000, 0.3300, 0.3600, 0.3900, 0.4200, 0.4500, 0.4800, 0.5100});
		auto phecDecreaseCond = samplesActivated && phecCount >= 2;
		auto pyroRes = Requires(phecDecreaseCond && ElementCount{Misc::Element::pyro} >= 1, resDecreaseMult);
		auto hydroRes = Requires(phecDecreaseCond && ElementCount{Misc::Element::hydro} >= 1, resDecreaseMult);
		auto electroRes = Requires(phecDecreaseCond && ElementCount{Misc::Element::electro} >= 1, resDecreaseMult);
		auto cryoRes = Requires(phecDecreaseCond && ElementCount{Misc::Element::cryo} >= 1, resDecreaseMult);
		auto geoRes = Requires((samplesActivated && phecCount <= 2) || Requirement::constellation2, resDecreaseMult);

		auto a1DmgIncrease = Requires(phecCount < 2 && Requirement::passive1, Constant(0.3f));

		auto a4DefIncrease = Requires(Requirement::passive2 && IsActive("xilonenNightsoulBurstTriggered"), Constant(0.2f));

		auto c2BuffCond = Requirement::constellation2 && samplesActivated;
		auto c2GeoBuff = Requires(c2BuffCond && IsActiveCharacterElement{Misc::Element::geo}, Constant(0.5f));
		auto c2PyroBuff = Requires(c2BuffCond && IsActiveCharacterElement{Misc::Element::pyro}, Constant(0.45f));
		auto c2HydroBuff = Requires(c2BuffCond && IsActiveCharacterElement{Misc::Element::hydro}, Constant(0.45f));
		auto c2CryoBuff = Requires(c2BuffCond && IsActiveCharacterElement{Misc::Element::cryo}, Constant(0.60f));

		auto c4DmgBuff = total.def * Requires(Requirement::constellation4 && IsActive("xilonenC4AfterUse"), Constant(0.65f));

		auto c6Imperishable = IsActive("xilonenC6Imperishable");
		auto c6Buff = Requires(Requirement::constellation6 && c6Imperishable, total.def * 3.f);
		auto c6Healing = Requires(Requirement::constellation6, total.def * 1.2f);

		return Data::Setup{
			.mods{
				.preMod{
					.def_ = a4DefIncrease,
					.normal{
						.DMG = a1DmgIncrease,
						.additiveDMG = c6Buff,
					},
					.plunge{
						.DMG = a1DmgIncrease,
						.additiveDMG = c6Buff,
					},
				},
				.teamPreMod{
					.hp_ = c2HydroBuff,
					.atk_ = c2PyroBuff,
					.cd = c2CryoBuff,
					.all{.DMG = c2GeoBuff},
					.normal{.additiveDMG = c4DmgBuff},
					.charged{.additiveDMG = c4DmgBuff},
					.plunge{.additiveDMG = c4DmgBuff},
				},
				.enemy{
					.resistance{
						.pyro = pyroRes,
						.hydro = hydroRes,
						.cryo = cryoRes,
						.electro = electroRes,
						.geo = geoRes,
					},
				},
			},
			.opts{
				.skill{
					Option::Boolean{
						.key = "xilonenSamplesActivated",
						.name = "Source Samples activated",
						.teamBuff = true,
						.mods{
							.teamPreMod{
								.hp_ = c2HydroBuff,
								.atk_ = c2PyroBuff,
								.cd = c2CryoBuff,
								.all{.DMG = c2GeoBuff},
							},
							.enemy{
								.resistance{
									.pyro = pyroRes,
									.hydro = hydroRes,
									.cryo = cryoRes,
									.electro = electroRes,
									.geo = geoRes,
								},
							},
						},
					},
				},
				.passive2{
					Option::Boolean{
						.key = "xilonenNightsoulBurstTriggered",
						.name = "Nearby party members triggered a Nightsoul Burst",
						.teamBuff = true,
						.mods{
							.preMod{
								.def_ = a4DefIncrease,
							},
						},
					},
				},
				.constellation4{
					Option::Boolean{
						.key = "xilonenC4AfterUse",
						.name = "After Xilonen uses Yohual's Scratch",
						.teamBuff = true,
						.mods{
							.teamPreMod{
								.normal{.additiveDMG = c4DmgBuff},
								.charged{.additiveDMG = c4DmgBuff},
								.plunge{.additiveDMG = c4DmgBuff},
							},
						},
					},
				},
				.constellation6{
					Option::Boolean{
						.key = "xilonenC6Imperishable",
						.name = "Imperishable Night's Blessing",
						.mods{
							.preMod{
								.normal{.additiveDMG = c6Buff},
								.plunge{.additiveDMG = c6Buff},
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
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.5179, 0.5601, 0.6022, 0.6625, 0.7046, 0.7528, 0.8190, 0.8853, 0.9515, 1.0238, 1.0961, 1.1683, 1.2406, 1.3129, 1.3851}),
					},
					Node::Atk{
						.name = "2-Hit DMG (1)",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.2737, 0.2960, 0.3183, 0.3501, 0.3724, 0.3979, 0.4329, 0.4679, 0.5029, 0.5411, 0.5793, 0.6175, 0.6557, 0.6939, 0.7321}),
					},
					Node::Atk{
						.name = "2-Hit DMG (2)",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.2737, 0.2960, 0.3183, 0.3501, 0.3724, 0.3979, 0.4329, 0.4679, 0.5029, 0.5411, 0.5793, 0.6175, 0.6557, 0.6939, 0.7321}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.7295, 0.7889, 0.8482, 0.9331, 0.9925, 1.0603, 1.1536, 1.2469, 1.3402, 1.4420, 1.5438, 1.6456, 1.7474, 1.8492, 1.9510}),
					},
					Node::Atk{
						.name = "Blade Roller 1-Hit DMG",
						.element = Misc::Element::geo,
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.def, LevelableTalent::normal, {0.5602, 0.6058, 0.6514, 0.7166, 0.7622, 0.8143, 0.8859, 0.9576, 1.0292, 1.1074, 1.1856, 1.2638, 1.3419, 1.4201, 1.4983}),
					},
					Node::Atk{
						.name = "Blade Roller 2-Hit DMG",
						.element = Misc::Element::geo,
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.def, LevelableTalent::normal, {0.5505, 0.5953, 0.6401, 0.7041, 0.7489, 0.8001, 0.8705, 0.9409, 1.0113, 1.0882, 1.1650, 1.2418, 1.3186, 1.3954, 1.4722}),
					},
					Node::Atk{
						.name = "Blade Roller 3-Hit DMG",
						.element = Misc::Element::geo,
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.def, LevelableTalent::normal, {0.6582, 0.7117, 0.7653, 0.8418, 0.8954, 0.9566, 1.0408, 1.1250, 1.2092, 1.3010, 1.3928, 1.4847, 1.5765, 1.6684, 1.7602}),
					},
					Node::Atk{
						.name = "Blade Roller 4-Hit DMG",
						.element = Misc::Element::geo,
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.def, LevelableTalent::normal, {0.8603, 0.9303, 1.0003, 1.1004, 1.1704, 1.2504, 1.3604, 1.4705, 1.5805, 1.7005, 1.8206, 1.9406, 2.0607, 2.1807, 2.3007}),
					},
				},
				.charged{
					Node::Atk{
						.name = "Charged Attack DMG",
						.source = Misc::AttackSource::charged,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.9133, 0.9877, 1.0620, 1.1682, 1.2425, 1.3275, 1.4443, 1.5611, 1.6780, 1.8054, 1.9328, 2.0603, 2.1877, 2.3152, 2.4426}),
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
						.name = "Rush DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.def, LevelableTalent::skill, {1.7920, 1.9264, 2.0608, 2.2400, 2.3744, 2.5088, 2.6880, 2.8672, 3.0464, 3.2256, 3.4048, 3.5840, 3.8080, 4.0320, 4.2560}),
					},
					Node::Info{
						.name = "Elemental RES Decrease",
						.type = EntryType::multiplier,
						.formula = resDecreaseMult,
					},
					Node::Info{
						.name = "Source Sample Activation Duration",
						.type = EntryType::seconds,
						.formula = Multiplier(EntryType::seconds, LevelableTalent::skill, {15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000}),
					},
					Node::Info{
						.name = "Nightsoul Point Time Limit",
						.type = EntryType::seconds,
						.formula = Multiplier(EntryType::seconds, LevelableTalent::skill, {9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000, 9.0000}),
					},
					Node::Info{
						.name = "Nightsoul Point Limit",
						.formula = Multiplier(LevelableTalent::skill, {90.0000, 90.0000, 90.0000, 90.0000, 90.0000, 90.0000, 90.0000, 90.0000, 90.0000, 90.0000, 90.0000, 90.0000, 90.0000, 90.0000, 90.0000}),
					},
					Node::Info{
						.name = "CD",
						.type = EntryType::seconds,
						.formula = Multiplier(EntryType::seconds, LevelableTalent::skill, {7.0000, 7.0000, 7.0000, 7.0000, 7.0000, 7.0000, 7.0000, 7.0000, 7.0000, 7.0000, 7.0000, 7.0000, 7.0000, 7.0000, 7.0000}),
					},
				},
				.burst{
					Node::Atk{
						.name = "Skill DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.def, LevelableTalent::burst, {2.8128, 3.0238, 3.2347, 3.5160, 3.7270, 3.9379, 4.2192, 4.5005, 4.7818, 5.0630, 5.3443, 5.6256, 5.9772, 6.3288, 6.6804}),
					},
					Node::Heal{
						.name = "Continuous Healing",
						.formula = Multiplier(total.def, LevelableTalent::burst, {1.0400, 1.1180, 1.1960, 1.3000, 1.3780, 1.4560, 1.5600, 1.6640, 1.7680, 1.8720, 1.9760, 2.0800, 2.2100, 2.3400, 2.4700})
								 + Multiplier(EntryType::points, LevelableTalent::burst, {500.7376, 550.8184, 605.0725, 663.5000, 726.1009, 792.8752, 863.8229, 938.9440, 1018.2385, 1101.7063, 1189.3477, 1281.1622, 1377.1503, 1477.3118, 1581.6466}),
					},
					Node::Info{
						.name = "Ebullient Rhythm Duration",
						.type = EntryType::seconds,
						.formula = Multiplier(EntryType::seconds, LevelableTalent::burst, {12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000, 12.0000}),
					},
					Node::Atk{
						.name = "Follow-Up Beat DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.def, LevelableTalent::burst, {2.8128, 3.0238, 3.2347, 3.5160, 3.7270, 3.9379, 4.2192, 4.5005, 4.7818, 5.0630, 5.3443, 5.6256, 5.9772, 6.3288, 6.6804}),
					},
					Node::Info{
						.name = "CD",
						.type = EntryType::seconds,
						.formula = Multiplier(EntryType::seconds, LevelableTalent::burst, {15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000}),
					},
					Node::Info{
						.name = "Energy Cost",
						.formula = Multiplier(LevelableTalent::burst, {60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000, 60.0000}),
					},
				},
				.passive1{
					Node::Info{
						.name = "Normal Attack DMG Increase",
						.formula = a1DmgIncrease,
					},
					Node::Info{
						.name = "Charged Attack DMG Increase",
						.formula = a1DmgIncrease,
					},
				},
				.constellation6{
					Node::Heal{
						.name = "Imperishable Night's Blessing Healing",
						.formula = c6Healing,
					},
				},
			},
		};
	},
};
