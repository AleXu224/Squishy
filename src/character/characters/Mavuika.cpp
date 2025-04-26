#include "Mavuika.hpp"

#include "character/setup.hpp"
#include "ranges"

const Character::Data Character::Datas::mavuika{
	.key{10000106},
	.goodKey{"Mavuika"},
	.name{"Mavuika"},
	.baseStats{
		.baseHp = 977.153,
		.baseAtk = 27.930,
		.baseDef = 61.625,
		.ascensionStat = Stat::cd,
		.rarity = 5,
		.c3Talent = ConstellationTalent::burst,
		.c5Talent = ConstellationTalent::skill,
		.element = Misc::Element::pyro,
		.weaponType = Misc::WeaponType::claymore,
		.hpCurve = Curves::CharacterGrow::HP_S5,
		.atkCurve = Curves::CharacterGrow::ATTACK_S5,
		.defCurve = Curves::CharacterGrow::HP_S5,
		.hpUpgrade = {0.000, 837.820, 1433.114, 2226.838, 2822.132, 3417.425, 4012.719},
		.atkUpgrade = {0.000, 23.946, 40.960, 63.645, 80.659, 97.673, 114.687},
		.defUpgrade = {0.000, 52.839, 90.382, 140.441, 177.984, 215.527, 253.071},
		.ascensionStatUpgrade = {0.000, 0.000, 0.096, 0.192, 0.192, 0.288, 0.384},
	},
	.setup = []() -> Data::Setup {
		auto burstIncreaseMultiplier = Multiplier(Utils::EntryType::multiplier, LevelableTalent::burst, {0.0160, 0.0172, 0.0184, 0.0200, 0.0212, 0.0224, 0.0240, 0.0256, 0.0272, 0.0288, 0.0304, 0.0320, 0.0340, 0.0360, 0.0380});
		auto normalIncreaseMultiplier = Multiplier(Utils::EntryType::multiplier, LevelableTalent::burst, {0.0026, 0.0028, 0.0030, 0.0033, 0.0035, 0.0037, 0.0041, 0.0044, 0.0047, 0.0051, 0.0055, 0.0058, 0.0062, 0.0065, 0.0069});
		auto chargedIncreaseMultiplier = Multiplier(Utils::EntryType::multiplier, LevelableTalent::burst, {0.0052, 0.0056, 0.0060, 0.0066, 0.0070, 0.0075, 0.0082, 0.0088, 0.0095, 0.0102, 0.0109, 0.0116, 0.0124, 0.0131, 0.0138});

		auto burstActive = IsActive("mavuikaBurstStacks");
		auto burstStacks = GetInt("mavuikaBurstStacks");
		auto burstIncrease = burstStacks * burstIncreaseMultiplier * total.atk;
		auto normalIncrease = burstStacks * normalIncreaseMultiplier * total.atk;
		auto chargedIncrease = burstStacks * chargedIncreaseMultiplier * total.atk;

		auto a1 = IsActive("mavuikaA1");
		auto a1Buff = Requires(a1 && Requirement::passive1, Constant(0.3f));

		auto c4 = Requirement::constellation4;
		auto c4Buff = Requires(c4, Constant(0.1f));

		auto a4 = IsActive("mavuikaA4");
		auto a4Stacks = GetInt("mavuikaA4");
		auto a4Decay = IfElse(
			c4,
			Constant(1.f),
			((ConstantFlat(20.f) - a4Stacks) / ConstantFlat(20.f))
		);
		auto a4Buff = Requires(
			burstActive && a4 && Requirement::passive2,
			c4Buff + burstStacks * 0.002f * a4Decay
		);

		auto c1 = IsActive("mavuikaC1");
		auto c1Buff = Requires(c1 && Requirement::constellation1, Constant(0.4f));

		auto c2Ring = IsActive("mavuikaC2Ring");
		auto c2Flamestrider = IsActive("mavuikaC2Flamestrider");
		auto c2BaseBuff = Requires(Requirement::constellation2 && (c2Ring || c2Flamestrider), ConstantFlat(200.f));
		auto c2RingBuff = Requires(Requirement::constellation2 && c2Ring, Constant(0.2f));
		auto c2NormalFlamestriderBuff = Requires(Requirement::constellation2 && c2Flamestrider, 0.6f * total.atk);
		auto c2ChargedFlamestriderBuff = Requires(Requirement::constellation2 && c2Flamestrider, 0.9f * total.atk);
		auto c2BurstFlamestriderBuff = Requires(Requirement::constellation2 && c2Flamestrider, 1.2f * total.atk);

		auto c6DefRes = Requires(Requirement::constellation6 && c2Flamestrider, Constant(0.2f));

		return Data::Setup{
			.mods{
				.preMod{
					.atk_ = a1Buff + c1Buff,
					.baseAtk = c2BaseBuff,
				},
				.teamPreMod{
					.all{.DMG = a4Buff},
				},
				.enemy{
					.DEFReduction = c2RingBuff + c6DefRes,
				},
			},
			.opts{
				.burst{
					Option::ValueList{
						.key = "mavuikaBurstStacks",
						.prefix = "Fighting Spirit consumed",
						.teamBuff = true,
						.values = std::views::iota(100 / 10)
								| std::views::take(11)
								| std::views::transform([](auto &&val) {
									  return val * 10;
								  })
								| std::ranges::to<std::vector<uint32_t>>(),
						.nodes{
							Node::Info{
								.name = "Elemental Burst DMG Increase",
								.formula = burstIncrease,
							},
							Node::Info{
								.name = "Flamestrider Normal Attack DMG Increase",
								.formula = normalIncrease,
							},
							Node::Info{
								.name = "Flamestrider Charged Attack DMG Increase",
								.formula = chargedIncrease,
							},
						},
					},
				},
				.passive1{
					Option::Boolean{
						.key = "mavuikaA1",
						.name = "Nearby party member triggered a Nightsoul Burst",
						.mods{
							.preMod{
								.atk_ = a1Buff,
							},
						},
					},
				},
				.passive2{
					Option::ValueList{
						.key = "mavuikaA4",
						.prefix = "Time since burst used",
						.displayCondition = burstActive,
						.values = std::views::iota(0) | std::views::take(20) | std::ranges::to<std::vector<uint32_t>>(),
						.mods{
							.teamPreMod{
								.all{.DMG = a4Buff},
							},
						},
					},
				},
				.constellation1{
					Option::Boolean{
						.key = "mavuikaC1",
						.name = "After gaining fighting spirit",
						.mods{
							.preMod{
								.atk_ = c1Buff,
							},
						},
					},
				},
				.constellation2{
					Option::Boolean{
						.key = "mavuikaC2Ring",
						.name = "Ring of Searing Radiance active",
						.displayCondition = !c2Flamestrider,
						.mods{
							.preMod{
								.baseAtk = c2BaseBuff,
							},
							.enemy{
								.DEFReduction = c2RingBuff,
							},
						},
					},
					Option::Boolean{
						.key = "mavuikaC2Flamestrider",
						.name = "Flamestrider active",
						.displayCondition = !c2Ring,
						.mods{
							.preMod{
								.baseAtk = c2BaseBuff,
							},
							.enemy{
								.DEFReduction = c6DefRes,
							},
						},
						.nodes{
							Node::Info{
								.name = "Elemental Burst DMG Increase",
								.formula = c2BurstFlamestriderBuff,
							},
							Node::Info{
								.name = "Flamestrider Normal Attack DMG Increase",
								.formula = c2NormalFlamestriderBuff,
							},
							Node::Info{
								.name = "Flamestrider Charged Attack DMG Increase",
								.formula = c2ChargedFlamestriderBuff,
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
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.8004, 0.8655, 0.9306, 1.0237, 1.0888, 1.1633, 1.2657, 1.3680, 1.4704, 1.5821, 1.6938, 1.8054, 1.9171, 2.0288, 2.1405}),
					},
					Node::Atk{
						.name = "2-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.3648, 0.3945, 0.4242, 0.4666, 0.4963, 0.5302, 0.5769, 0.6236, 0.6702, 0.7211, 0.7720, 0.8229, 0.8738, 0.9247, 0.9756}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.3322, 0.3593, 0.3863, 0.4249, 0.4520, 0.4829, 0.5254, 0.5679, 0.6104, 0.6567, 0.7031, 0.7495, 0.7958, 0.8422, 0.8885}),
					},
					Node::Atk{
						.name = "4-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.1619, 1.2565, 1.3511, 1.4862, 1.5808, 1.6888, 1.8375, 1.9861, 2.1347, 2.2968, 2.4590, 2.6211, 2.7832, 2.9454, 3.1075}),
					},
				},
				.charged{
					Node::Atk{
						.name = "Charged Attack DMG",
						.source = Misc::AttackSource::charged,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.9384, 2.0962, 2.2540, 2.4794, 2.6372, 2.8175, 3.0654, 3.3134, 3.5613, 3.8318, 4.1023, 4.3728, 4.6432, 4.9137, 5.1842}),
					},
					Node::Info{
						.name = "Charged Attack Stamina Cost",
						.formula = Multiplier(LevelableTalent::normal, {50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000}),
					},
				},
				.plunge{
					Node::Atk{
						.name = "Plunge DMG",
						.source = Misc::AttackSource::plunge,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.7459, 0.8066, 0.8673, 0.9540, 1.0147, 1.0841, 1.1795, 1.2749, 1.3703, 1.4744, 1.5785, 1.6826, 1.7866, 1.8907, 1.9948}),
					},
					Node::Atk{
						.name = "Low Plunge DMG",
						.source = Misc::AttackSource::plunge,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.4914, 1.6128, 1.7342, 1.9077, 2.0291, 2.1678, 2.3586, 2.5493, 2.7401, 2.9482, 3.1563, 3.3644, 3.5725, 3.7806, 3.9887}),
					},
					Node::Atk{
						.name = "High Plunge DMG",
						.source = Misc::AttackSource::plunge,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.8629, 2.0145, 2.1662, 2.3828, 2.5344, 2.7077, 2.9460, 3.1842, 3.4225, 3.6825, 3.9424, 4.2023, 4.4623, 4.7222, 4.9821}),
					},
				},
				.skill{
					Node::Atk{
						.name = "Skill DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.7440, 0.7998, 0.8556, 0.9300, 0.9858, 1.0416, 1.1160, 1.1904, 1.2648, 1.3392, 1.4136, 1.4880, 1.5810, 1.6740, 1.7670}),
					},
					Node::Atk{
						.name = "Ring of Searing Radiance DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {1.2800, 1.3760, 1.4720, 1.6000, 1.6960, 1.7920, 1.9200, 2.0480, 2.1760, 2.3040, 2.4320, 2.5600, 2.7200, 2.8800, 3.0400}),
					},
					Node::Info{
						.name = "Ring of Searing Radiance Attack Interval",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(LevelableTalent::skill, {2.0000, 2.0000, 2.0000, 2.0000, 2.0000, 2.0000, 2.0000, 2.0000, 2.0000, 2.0000, 2.0000, 2.0000, 2.0000, 2.0000, 2.0000}),
					},
					Node::Atk{
						.name = "Flamestrider Normal Attack 1-Hit DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.5726, 0.6193, 0.6659, 0.7325, 0.7791, 0.8323, 0.9056, 0.9788, 1.0521, 1.1320, 1.2119, 1.2918, 1.3717, 1.4516, 1.5315}),
						.modifier{
							.additiveDMG = normalIncrease + c2NormalFlamestriderBuff,
						},
					},
					Node::Atk{
						.name = "Flamestrider Normal Attack 2-Hit DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.5913, 0.6395, 0.6876, 0.7563, 0.8045, 0.8595, 0.9351, 1.0108, 1.0864, 1.1689, 1.2514, 1.3339, 1.4164, 1.4989, 1.5815}),
						.modifier{
							.additiveDMG = normalIncrease + c2NormalFlamestriderBuff,
						},
					},
					Node::Atk{
						.name = "Flamestrider Normal Attack 3-Hit DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.6999, 0.7568, 0.8138, 0.8952, 0.9521, 1.0172, 1.1068, 1.1963, 1.2858, 1.3835, 1.4811, 1.5788, 1.6764, 1.7741, 1.8717}),
						.modifier{
							.additiveDMG = normalIncrease + c2NormalFlamestriderBuff,
						},
					},
					Node::Atk{
						.name = "Flamestrider Normal Attack 4-Hit DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.6970, 0.7538, 0.8105, 0.8916, 0.9483, 1.0131, 1.1023, 1.1915, 1.2806, 1.3779, 1.4751, 1.5724, 1.6697, 1.7669, 1.8642}),
						.modifier{
							.additiveDMG = normalIncrease + c2NormalFlamestriderBuff,
						},
					},
					Node::Atk{
						.name = "Flamestrider Normal Attack 5-Hit DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.9100, 0.9841, 1.0582, 1.1640, 1.2381, 1.3227, 1.4391, 1.5555, 1.6719, 1.7989, 1.9259, 2.0529, 2.1799, 2.3068, 2.4338}),
						.modifier{
							.additiveDMG = normalIncrease + c2NormalFlamestriderBuff,
						},
					},
					Node::Atk{
						.name = "Flamestrider Sprint DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.8084, 0.8742, 0.9400, 1.0340, 1.0998, 1.1750, 1.2784, 1.3818, 1.4852, 1.5980, 1.7108, 1.8236, 1.9364, 2.0492, 2.1620}),
					},
					Node::Atk{
						.name = "Flamestrider Charged Attack Cyclic DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.9890, 1.0695, 1.1500, 1.2650, 1.3455, 1.4375, 1.5640, 1.6905, 1.8170, 1.9550, 2.0930, 2.2310, 2.3690, 2.5070, 2.6450}),
						.modifier{
							.additiveDMG = chargedIncrease + c2ChargedFlamestriderBuff,
						},
					},
					Node::Atk{
						.name = "Flamestrider Charged Attack Final DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {1.3760, 1.4880, 1.6000, 1.7600, 1.8720, 2.0000, 2.1760, 2.3520, 2.5280, 2.7200, 2.9120, 3.1040, 3.2960, 3.4880, 3.6800}),
						.modifier{
							.additiveDMG = chargedIncrease + c2ChargedFlamestriderBuff,
						},
					},
					Node::Atk{
						.name = "Flamestrider Plunge DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {1.5996, 1.7298, 1.8600, 2.0460, 2.1762, 2.3250, 2.5296, 2.7342, 2.9388, 3.1620, 3.3852, 3.6084, 3.8316, 4.0548, 4.2780}),
					},
					Node::Info{
						.name = "Nightsoul Point Limit",
						.formula = Multiplier(LevelableTalent::skill, {80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000, 80.0000}),
					},
					Node::Info{
						.name = "CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::skill, {15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000}),
					},
				},
				.burst{
					Node::Atk{
						.name = "Skill DMG",
						.source = Misc::AttackSource::burst,
						.formula = Multiplier(total.atk, LevelableTalent::burst, {4.4480, 4.7816, 5.1152, 5.5600, 5.8936, 6.2272, 6.6720, 7.1168, 7.5616, 8.0064, 8.4512, 8.8960, 9.4520, 10.0080, 10.5640}),
						.modifier{
							.additiveDMG = burstIncrease + c2BurstFlamestriderBuff,
						},
					},
					Node::Info{
						.name = "Crucible of Death and Life Duration",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(LevelableTalent::burst, {7.0000, 7.0000, 7.0000, 7.0000, 7.0000, 7.0000, 7.0000, 7.0000, 7.0000, 7.0000, 7.0000, 7.0000, 7.0000, 7.0000, 7.0000}),
					},
					Node::Info{
						.name = "Sunfell Slice DMG Bonus",
						.type = Utils::EntryType::multiplier,
						.formula = burstIncreaseMultiplier,
					},
					Node::Info{
						.name = "Flamestrider Normal Attack DMG Bonus",
						.type = Utils::EntryType::multiplier,
						.formula = normalIncreaseMultiplier,
					},
					Node::Info{
						.name = "Flamestrider Charged Attack DMG Bonus",
						.type = Utils::EntryType::multiplier,
						.formula = chargedIncreaseMultiplier,
					},
					Node::Info{
						.name = "CD",
						.type = Utils::EntryType::seconds,
						.formula = Multiplier(Utils::EntryType::seconds, LevelableTalent::burst, {18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000, 18.0000}),
					},
					Node::Info{
						.name = "Fighting Spirit Limit",
						.formula = Multiplier(LevelableTalent::burst, {200.0000, 200.0000, 200.0000, 200.0000, 200.0000, 200.0000, 200.0000, 200.0000, 200.0000, 200.0000, 200.0000, 200.0000, 200.0000, 200.0000, 200.0000}),
					},
				},
				.constellation6{
					Node::Atk{
						.name = "Flamestrider DMG",
						.source = Misc::AttackSource::skill,
						.formula = 2.f * total.atk,
					},
					Node::Atk{
						.name = "Scorching Ring of Searing Radiance DMG",
						.source = Misc::AttackSource::skill,
						.formula = 5.f * total.atk,
					},
				},
			},
		};
	},
};
