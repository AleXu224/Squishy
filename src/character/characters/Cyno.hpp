#pragma once

#include "character/setup.hpp"

namespace Character::Datas {
	inline const Character::Data cyno{
		.key{10000071},
		.name{"Cyno"},
		.baseStats{
			.baseHp = 972.3864,
			.baseAtk = 24.7646,
			.baseDef = 66.88994,
			.ascensionStat = Stat::cd,
			.c3Talent = ConstellationTalent::burst,
			.c5Talent = ConstellationTalent::skill,
			.element = Misc::Element::electro,
			.hpCurve = Curves::CharacterGrow::HP_S5,
			.atkCurve = Curves::CharacterGrow::ATTACK_S5,
			.defCurve = Curves::CharacterGrow::HP_S5,
			.hpUpgrade = {0, 833.7334, 1426.1229, 2215.9756, 2808.3652, 3400.7546, 3993.1443},
			.atkUpgrade = {0, 21.231853, 36.317646, 56.432034, 71.51782, 86.603615, 101.68941},
			.defUpgrade = {0, 57.3534, 98.1045, 152.4393, 193.1904, 233.9415, 274.6926},
			.ascensionStatUpgrade = {0, 0, 0.096, 0.192, 0.192, 0.288, 0.384},
		},
		.opts{
			.burst{
				Option::Boolean{
					.key = "burstActive",
					.name = "Burst Active",
				},
			},
			.passive1{
				Option::Boolean{
					.key = "endseerStance",
					.name = "During Endseer stance",
				},
			},
			.constellation2{
				Option::ValueList{
					.key = "c2Hits",
					.prefix = "Normal Attack Hits",
					.values{1, 2, 3, 4, 5},
				},
			},
		},
		.setup = []() -> Data::Setup {
			constexpr auto a1SkillBonus = Modifier{
				.DMG = Requires(
					Requirement::passive1 && IsActive("endseerStance"),
					Constant(0.35f)
				),
			};

			constexpr auto a4BurstBonus = Requires(Requirement::passive2, CharacterStat(Stat::em) * 1.5f);
			constexpr auto a4BoltBbonus = Requires(Requirement::passive2, CharacterStat(Stat::em) * 2.5f);
			constexpr auto a4BurstModifier = Modifier{
				.additiveDMG = a4BurstBonus,
			};

			return Data::Setup{
				.mods{
					.preMod{
						.em = Requires(
							IsActive("burstActive"),
							ConstantFlat(100.f)
						),
						.electro{
							.DMG = Requires(Requirement::constellation2, GetFloat("c2Hits") * 0.1f),
						},
					},
				},
				.nodes{
					.normal{
						Node::Atk{
							.name = "1-Hit DMG",
							.source = Misc::AttackSource::normal,
							.formula = Multiplier(Stat::atk, LevelableTalent::normal, {0.49257, 0.53267, 0.57276, 0.63004, 0.67013, 0.71595, 0.77895, 0.84196, 0.90496, 0.97369, 1.04242, 1.11115, 1.17989, 1.24862, 1.31735})
						},
						Node::Atk{
							.name = "2-Hit DMG",
							.source = Misc::AttackSource::normal,
							.formula = Multiplier(Stat::atk, LevelableTalent::normal, {0.47921, 0.51822, 0.55722, 0.61294, 0.65195, 0.69652, 0.75782, 0.81911, 0.88041, 0.94727, 1.01414, 1.08101, 1.14787, 1.21474, 1.28161})
						},
						Node::Atk{
							.name = "3-Hit DMG (2)",
							.source = Misc::AttackSource::normal,
							.formula = Multiplier(Stat::atk, LevelableTalent::normal, {0.29306, 0.31692, 0.34077, 0.37485, 0.3987, 0.42596, 0.46345, 0.50093, 0.53842, 0.57931, 0.6202, 0.66109, 0.70199, 0.74288, 0.78377})
						},
						Node::Atk{
							.name = "4-Hit DMG",
							.source = Misc::AttackSource::normal,
							.formula = Multiplier(Stat::atk, LevelableTalent::normal, {0.75891, 0.82068, 0.88245, 0.9707, 1.03247, 1.10306, 1.20013, 1.2972, 1.39427, 1.50016, 1.60606, 1.71195, 1.81785, 1.92374, 2.02963})
						},
					},
					.charged{
						Node::Atk{
							.name = "Charged Attack DMG",
							.source = Misc::AttackSource::charged,
							.formula = Multiplier(Stat::atk, LevelableTalent::normal, {1.22378, 1.32339, 1.423, 1.5653, 1.66491, 1.77875, 1.93528, 2.09181, 2.24834, 2.4191, 2.58986, 2.76062, 2.93138, 3.10214, 3.2729})
						},
					},
					.plunge{
						Node::Atk{
							.name = "Plunge DMG",
							.source = Misc::AttackSource::plunge,
							.formula = Multiplier(Stat::atk, LevelableTalent::normal, {0.63932, 0.69136, 0.7434, 0.81774, 0.86978, 0.92925, 1.01102, 1.0928, 1.17457, 1.26378, 1.35299, 1.4422, 1.5314, 1.62061, 1.70982})
						},
						Node::Atk{
							.name = "Low Plunge DMG",
							.source = Misc::AttackSource::plunge,
							.formula = Multiplier(Stat::atk, LevelableTalent::normal, {1.27838, 1.38243, 1.48649, 1.63513, 1.73919, 1.85811, 2.02162, 2.18513, 2.34865, 2.52702, 2.7054, 2.88378, 3.06216, 3.24054, 3.41892})
						},
						Node::Atk{
							.name = "High Plunge DMG",
							.source = Misc::AttackSource::plunge,
							.formula = Multiplier(Stat::atk, LevelableTalent::normal, {1.59676, 1.72673, 1.8567, 2.04237, 2.17234, 2.32087, 2.52511, 2.72935, 2.93359, 3.15639, 3.37919, 3.602, 3.8248, 4.04761, 4.27041})
						},
					},
					.skill{
						Node::Atk{
							.name = "Skill DMG",
							.source = Misc::AttackSource::skill,
							.formula = Multiplier(Stat::atk, LevelableTalent::skill, {1.304, 1.4018, 1.4996, 1.63, 1.7278, 1.8256, 1.956, 2.0864, 2.2168, 2.3472, 2.4776, 2.608, 2.771, 2.934, 3.097}),
							.modifier = a1SkillBonus,
						},
						Node::Atk{
							.name = "Mortuary Rite DMG",
							.source = Misc::AttackSource::skill,
							.formula = Multiplier(Stat::atk, LevelableTalent::skill, {1.568, 1.6856, 1.8032, 1.96, 2.0776, 2.1952, 2.352, 2.5088, 2.6656, 2.8224, 2.9792, 3.136, 3.332, 3.528, 3.724}),
							.modifier = a1SkillBonus,
						},
					},
					.burst{
						Node::Atk{
							.name = "1-Hit DMG",
							.element = Misc::Element::electro,
							.source = Misc::AttackSource::normal,
							.formula = Multiplier(Stat::atk, LevelableTalent::burst, {0.78283, 0.84655, 0.91027, 1.0013, 1.06502, 1.13784, 1.23797, 1.3381, 1.43823, 1.54746, 1.65669, 1.76592, 1.87516, 1.98439, 2.09362}),
							.modifier = a4BurstModifier,
						},
						Node::Atk{
							.name = "2-Hit DMG",
							.element = Misc::Element::electro,
							.source = Misc::AttackSource::normal,
							.formula = Multiplier(Stat::atk, LevelableTalent::burst, {0.82469, 0.89181, 0.95894, 1.05483, 1.12196, 1.19868, 1.30416, 1.40964, 1.51513, 1.6302, 1.74527, 1.86034, 1.97542, 2.09049, 2.20556}),
							.modifier = a4BurstModifier,
						},
						Node::Atk{
							.name = "3-Hit DMG",
							.element = Misc::Element::electro,
							.source = Misc::AttackSource::normal,
							.formula = Multiplier(Stat::atk, LevelableTalent::burst, {1.04634, 1.1315, 1.21667, 1.33834, 1.4235, 1.52084, 1.65467, 1.7885, 1.92234, 2.06834, 2.21434, 2.36034, 2.50634, 2.65234, 2.79834}),
							.modifier = a4BurstModifier,
						},
						Node::Atk{
							.name = "4-Hit DMG (2)",
							.element = Misc::Element::electro,
							.source = Misc::AttackSource::normal,
							.formula = Multiplier(Stat::atk, LevelableTalent::burst, {0.51694, 0.55902, 0.6011, 0.6612, 0.70328, 0.75137, 0.81749, 0.88361, 0.94973, 1.02186, 1.09399, 1.16612, 1.23826, 1.31039, 1.38252}),
							.modifier = a4BurstModifier,
						},
						Node::Atk{
							.name = "5-Hit DMG",
							.element = Misc::Element::electro,
							.source = Misc::AttackSource::normal,
							.formula = Multiplier(Stat::atk, LevelableTalent::burst, {1.30845, 1.41495, 1.52145, 1.67359, 1.7801, 1.90181, 2.06917, 2.23653, 2.40389, 2.58646, 2.76904, 2.95161, 3.13419, 3.31676, 3.49934}),
							.modifier = a4BurstModifier,
						},
					},
					.passive1{
						Node::Atk{
							.name = "Duststalker bolt DMG",
							.source = Misc::AttackSource::skill,
							.formula = CharacterStat(Stat::atk) * 1.f,
							.modifier = Modifier{
								.additiveDMG = a4BoltBbonus,
							}
						},
					},
					.passive2{
						Node::Info{
							.name = "Pactsworn Pathclearer's Normal Attack DMG Increase",
							.formula = a4BurstBonus,
						},
						Node::Info{
							.name = "Duststalker Bolt DMG Increase",
							.formula = a4BoltBbonus,
						},
					},
				},
			};
		},
	};
}// namespace Character::Datas
