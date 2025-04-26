#include "Nahida.hpp"

#include "character/setup.hpp"
#include "ranges"

const Character::Data Character::Datas::nahida{
	.key{10000073},
	.goodKey{"Nahida"},
	.name{"Nahida"},
	.baseStats{
		.baseHp = 806.509,
		.baseAtk = 23.275,
		.baseDef = 49.061,
		.ascensionStat = Stat::em,
		.rarity = 5,
		.c3Talent = ConstellationTalent::skill,
		.c5Talent = ConstellationTalent::burst,
		.element = Misc::Element::dendro,
		.weaponType = Misc::WeaponType::catalyst,
		.hpCurve = Curves::CharacterGrow::HP_S5,
		.atkCurve = Curves::CharacterGrow::ATTACK_S5,
		.defCurve = Curves::CharacterGrow::HP_S5,
		.hpUpgrade = {0.000, 691.508, 1182.843, 1837.956, 2329.291, 2820.626, 3311.961},
		.atkUpgrade = {0.000, 19.955, 34.133, 53.038, 67.216, 81.394, 95.573},
		.defUpgrade = {0.000, 42.066, 71.955, 111.807, 141.696, 171.585, 201.474},
		.ascensionStatUpgrade = {0.000, 0.000, 28.800, 57.600, 57.600, 86.400, 115.200},
	},
	.setup = []() -> Data::Setup {
		auto insideField = IsActive("nahidaInsideField");
		auto pyroCount = Max(ElementCount{Misc::Element::pyro} + Requires(Requirement::constellation1, ConstantInt(1)), 2);

		auto burst1PyroCharacters = Multiplier(LevelableTalent::burst, {0.1488, 0.1600, 0.1711, 0.1860, 0.1972, 0.2083, 0.2232, 0.2381, 0.2530, 0.2678, 0.2827, 0.2976, 0.3162, 0.3348, 0.3534});
		auto burst2PyroCharacters = Multiplier(LevelableTalent::burst, {0.2232, 0.2399, 0.2567, 0.2790, 0.2957, 0.3125, 0.3348, 0.3571, 0.3794, 0.4018, 0.4241, 0.4464, 0.4743, 0.5022, 0.5301});
		auto burstPyroMultiplier = IfElse{
			pyroCount == 1,
			burst1PyroCharacters,
			IfElse{
				pyroCount == 2,
				burst2PyroCharacters,
				Constant(0.f),
			},
		};
		auto burstSkillBuff = Requires(insideField, burstPyroMultiplier);

		auto maxEm = MaxCharacter{preMods.em};
		auto a1EmBuff = Requires(
			Requirement::passive1 && insideField,
			Min(0.25f * maxEm, 250.f)
		);

		auto a4Points = Clamp(total.em - ConstantFlat(200.f), 0.f, 800.f);
		auto a4TriKarmaDmg = Requires(Requirement::passive2, a4Points * 0.001);
		auto a4TriKarmaCr = Requires(Requirement::passive2, a4Points * 0.0003);

		auto c2OpponentMarked = IsActive("nahidaC2OpponentsMarked");
		auto c2ReactionTriggered = c2OpponentMarked && IsActive("nahidaC2ReactionTriggered");
		auto c2CritRate = Requires(Requirement::constellation2 && c2OpponentMarked, Constant(0.2f));
		auto c2CritDMG = Requires(Requirement::constellation2 && c2OpponentMarked, Constant(1.f));
		auto c2EnemyDef = Requires(Requirement::constellation2 && c2ReactionTriggered, Constant(0.3f));

		auto c4EmBuff = Requires(
			Requirement::constellation4 && IsActive("nahidaC4OpponentsAffected"),
			Index{
				GetInt("nahidaC4OpponentsAffected", 1) - ConstantInt(1),
				false,
				std::array{100.f, 120.f, 140.f, 160.f},
			}
		);

		return Data::Setup{
			.mods{
				.postMod{
					.em = c4EmBuff,
				},
				.teamPreMod{
					.burning{.critRate = c2CritRate, .critDMG = c2CritDMG},
					.bloom{.critRate = c2CritRate, .critDMG = c2CritDMG},
					.burgeon{.critRate = c2CritRate, .critDMG = c2CritDMG},
					.hyperbloom{.critRate = c2CritRate, .critDMG = c2CritDMG},
				},
				.teamPostMod{
					.em = a1EmBuff,
				},
				.enemy{
					.DEFReduction = c2EnemyDef,
				},
			},
			.opts{
				.burst{
					Option::Boolean{
						.key = "nahidaInsideField",
						.name = "Inside the Shrine of Maya",
						.teamBuff = true,
						.mods{
							.teamPostMod{
								.em = a1EmBuff,
							},
						},
					},
				},
				.constellation2{
					Option::Boolean{
						.key = "nahidaC2OpponentsMarked",
						.name = "Opponent Marked by Seeds of Skandha",
						.teamBuff = true,
						.mods{
							.teamPreMod{
								.burning{.critRate = c2CritRate, .critDMG = c2CritDMG},
								.bloom{.critRate = c2CritRate, .critDMG = c2CritDMG},
								.burgeon{.critRate = c2CritRate, .critDMG = c2CritDMG},
								.hyperbloom{.critRate = c2CritRate, .critDMG = c2CritDMG},
							},
						},
					},
					Option::Boolean{
						.key = "nahidaC2ReactionTriggered",
						.name = "Opponent affected by Quicken, Aggravate or Spread",
						.teamBuff = true,
						.displayCondition = c2OpponentMarked,
						.mods{
							.enemy{
								.DEFReduction = c2EnemyDef,
							},
						},
					},
				},
				.constellation4{
					Option::ValueList{
						.key = "nahidaC4OpponentsAffected",
						.prefix = "Opponents affected by All Schemes to Know's Seeds of Skandha",
						.teamBuff = true,
						.values = std::views::iota(1, 5) | std::ranges::to<std::vector<uint32_t>>(),
						.mods{
							.preMod{
								.em = c4EmBuff,
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
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4030, 0.4333, 0.4635, 0.5038, 0.5340, 0.5643, 0.6046, 0.6449, 0.6852, 0.7255, 0.7658, 0.8061, 0.8565, 0.9069, 0.9572}),
					},
					Node::Atk{
						.name = "2-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.3697, 0.3975, 0.4252, 0.4622, 0.4899, 0.5176, 0.5546, 0.5916, 0.6286, 0.6655, 0.7025, 0.7395, 0.7857, 0.8319, 0.8781}),
					},
					Node::Atk{
						.name = "3-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.4587, 0.4931, 0.5276, 0.5734, 0.6078, 0.6422, 0.6881, 0.7340, 0.7799, 0.8257, 0.8716, 0.9175, 0.9748, 1.0322, 1.0895}),
					},
					Node::Atk{
						.name = "4-Hit DMG",
						.source = Misc::AttackSource::normal,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {0.5841, 0.6279, 0.6717, 0.7301, 0.7739, 0.8177, 0.8761, 0.9345, 0.9929, 1.0513, 1.1097, 1.1681, 1.2411, 1.3141, 1.3872}),
					},
				},
				.charged{
					Node::Atk{
						.name = "Charged Attack DMG",
						.source = Misc::AttackSource::charged,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.3200, 1.4190, 1.5180, 1.6500, 1.7490, 1.8480, 1.9800, 2.1120, 2.2440, 2.3760, 2.5080, 2.6400, 2.8050, 2.9700, 3.1350}),
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
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.1363, 1.2288, 1.3213, 1.4535, 1.5459, 1.6516, 1.7970, 1.9423, 2.0877, 2.2462, 2.4048, 2.5634, 2.7219, 2.8805, 3.0390}),
					},
					Node::Atk{
						.name = "High Plunge DMG",
						.source = Misc::AttackSource::plunge,
						.formula = Multiplier(total.atk, LevelableTalent::normal, {1.4193, 1.5349, 1.6504, 1.8154, 1.9310, 2.0630, 2.2445, 2.4261, 2.6076, 2.8057, 3.0037, 3.2018, 3.3998, 3.5979, 3.7959}),
					},
				},
				.skill{
					Node::Atk{
						.name = "Press DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {0.9840, 1.0578, 1.1316, 1.2300, 1.3038, 1.3776, 1.4760, 1.5744, 1.6728, 1.7712, 1.8696, 1.9680, 2.0910, 2.2140, 2.3370}),
						.modifier = Modifier{
							.DMG = burstSkillBuff,
						},
					},
					Node::Atk{
						.name = "Hold DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {1.3040, 1.4018, 1.4996, 1.6300, 1.7278, 1.8256, 1.9560, 2.0864, 2.2168, 2.3472, 2.4776, 2.6080, 2.7710, 2.9340, 3.0970}),
						.modifier = Modifier{
							.DMG = burstSkillBuff,
						},
					},
					Node::Atk{
						.name = "Tri-Karma Purification DMG",
						.source = Misc::AttackSource::skill,
						.formula = Multiplier(total.atk, LevelableTalent::skill, {1.0320, 1.1094, 1.1868, 1.2900, 1.3674, 1.4448, 1.5480, 1.6512, 1.7544, 1.8576, 1.9608, 2.0640, 2.1930, 2.3220, 2.4510}) + Multiplier(total.em, LevelableTalent::skill, {2.0640, 2.2188, 2.3736, 2.5800, 2.7348, 2.8896, 3.0960, 3.3024, 3.5088, 3.7152, 3.9216, 4.1280, 4.3860, 4.6440, 4.9020}),
						.modifier = Modifier{
							.DMG = burstSkillBuff + a4TriKarmaDmg,
							.critRate = a4TriKarmaCr,
						},
					},
					Node::Info{
						.name = "Tri-Karma Purification Trigger Interval",
						.type = EntryType::seconds,
						.formula = Multiplier(LevelableTalent::skill, {2.5000, 2.5000, 2.5000, 2.5000, 2.5000, 2.5000, 2.5000, 2.5000, 2.5000, 2.5000, 2.5000, 2.5000, 2.5000, 2.5000, 2.5000}),
					},
					Node::Info{
						.name = "Seed of Skandha Duration",
						.type = EntryType::seconds,
						.formula = Multiplier(LevelableTalent::skill, {25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000, 25.0000}),
					},
					Node::Info{
						.name = "Press CD",
						.type = EntryType::seconds,
						.formula = Multiplier(LevelableTalent::skill, {5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000, 5.0000}),
					},
					Node::Info{
						.name = "Hold CD",
						.type = EntryType::seconds,
						.formula = Multiplier(LevelableTalent::skill, {6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000, 6.0000}),
					},
				},
				.burst{
					Node::Info{
						.name = "Base Duration",
						.type = EntryType::seconds,
						.formula = Multiplier(LevelableTalent::burst, {15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000, 15.0000}),
					},
					Node::Info{
						.name = "CD",
						.type = EntryType::seconds,
						.formula = Multiplier(LevelableTalent::burst, {13.5000, 13.5000, 13.5000, 13.5000, 13.5000, 13.5000, 13.5000, 13.5000, 13.5000, 13.5000, 13.5000, 13.5000, 13.5000, 13.5000, 13.5000}),
					},
					Node::Info{
						.name = "Energy Cost",
						.formula = Multiplier(LevelableTalent::burst, {50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000, 50.0000}),
					},
				},
				.passive2{
					Node::Info{
						.name = "Tri-Karma Purification Crit Rate",
						.type = EntryType::multiplier,
						.optimizable = true,
						.formula = a4TriKarmaCr,
					},
					Node::Info{
						.name = "Tri-Karma Purification DMG Increase",
						.type = EntryType::multiplier,
						.optimizable = true,
						.formula = a4TriKarmaDmg,
					},
				},
				.constellation6{
					Node::Atk{
						.name = "Karmic Oblivion DMG",
						.source = Misc::AttackSource::skill,
						.formula = total.atk * 2.f + total.em * 4.f,
						.modifier = Modifier{
							.DMG = burstSkillBuff + a4TriKarmaDmg,
							.critRate = a4TriKarmaCr,
						},
					},
				},
			},
		};
	},
};
