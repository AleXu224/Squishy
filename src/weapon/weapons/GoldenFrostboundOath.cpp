#include "GoldenFrostboundOath.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::goldenFrostboundOath{
	.key{15516},
	.goodKey{"GoldenFrostboundOath"},
	.name = "Golden Frostbound Oath",
	.baseStats{
		.type = Misc::WeaponType::bow,
		.rarity = 5,
		.baseAtk = 44.336,
		.atkCurve = Curves::WeaponGrow::ATTACK_304,
		.subStat = SubStat{
			.stat{
				.stat = Stat::cd,
				.value = 0.192,
			},
			.curve = Curves::WeaponGrow::CRITICAL_301,
		},
		.ascensionUpgrade{0.0, 31.1, 62.2, 93.4, 124.5, 155.6, 186.7}
	},
	.setup = []() {
		auto multiplier2 = WeaponMultiplier(true, {0.4000, 0.5000, 0.6000, 0.7000, 0.8000});
		auto multiplier3 = WeaponMultiplier(true, {0.4000, 0.5000, 0.6000, 0.7000, 0.8000});
		auto multiplier5 = WeaponMultiplier(true, {0.1600, 0.2000, 0.2400, 0.2800, 0.3200});
		auto multiplier6 = WeaponMultiplier(true, {0.2000, 0.2500, 0.3000, 0.3500, 0.4000});

		auto cond = IsActive("goldenFrostboundOathCond");
		auto buffGeo = Requires{cond, multiplier2};
		auto buffLunar = Requires{cond, multiplier3};

		auto cond2 = IsActive("goldenFrostboundOathMoondriftsNearby");
		auto teamBuffGeo = Requires{cond && cond2 && !Requirement::selfBuff, multiplier6};
		auto teamBuffLunar = Requires{cond && cond2 && !Requirement::selfBuff, multiplier6};

		return Data::Setup{
			.mods{
				.preMod{
					.def_ = multiplier5,
					.geo{
						.DMG = buffGeo,
					},
					.lunarCrystallize{
						.DMG = buffLunar,
					},
				},
				.teamPreMod{
					.geo{
						.DMG = teamBuffGeo,
					},
					.lunarCrystallize{
						.DMG = teamBuffLunar,
					},
				},
			},
			.opts{
				Option::Boolean{
					.key = "goldenFrostboundOathCond",
					.name = "The equipping character's Elemental Skill or Lunar-Crystallize attacks hit opponents",
					.teamBuff = true,
					.mods{
						.preMod{
							.geo{
								.DMG = buffGeo,
							},
							.lunarCrystallize{
								.DMG = buffLunar,
							},
						},
					},
				},
				Option::Boolean{
					.key = "goldenFrostboundOathMoondriftsNearby",
					.name = "Moondrifts near the equipping character",
					.teamBuff = true,
					.displayCondition = cond,
					.mods{
						.teamPreMod{
							.geo{
								.DMG = teamBuffGeo,
							},
							.lunarCrystallize{
								.DMG = teamBuffLunar,
							},
						},
					},
				},
			},
		};
	},
};
