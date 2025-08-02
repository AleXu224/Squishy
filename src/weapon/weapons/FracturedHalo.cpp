#include "FracturedHalo.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::fracturedHalo{
	.key{13515},
	.goodKey{"FracturedHalo"},
	.name = "Fractured Halo",
	.baseStats{
		.type = Misc::WeaponType::polearm,
		.rarity = 5,
		.baseAtk = 45.936,
		.atkCurve = Curves::WeaponGrow::ATTACK_301,
		.subStat = SubStat{
			.stat{
				.stat = Stat::cd,
				.value = 0.144,
			},
			.curve = Curves::WeaponGrow::CRITICAL_301,
		},
		.ascensionUpgrade{0.0, 31.1, 62.2, 93.4, 124.5, 155.6, 186.7}
	},
	.setup = []() {
		auto multiplier1 = WeaponMultiplier(true, {0.2400, 0.3000, 0.3600, 0.4200, 0.4800});
		auto multiplier3 = WeaponMultiplier(true, {0.4000, 0.5000, 0.6000, 0.7000, 0.8000});

		auto cond1 = IsActive("fracturedHaloCond1");
		auto buff1 = Requires{
			cond1,
			multiplier1,
		};

		auto cond2 = IsActive("fracturedHaloCond2");
		auto buff2 = Requires{
			cond2 && cond1,
			multiplier3,
		};

		return Data::Setup{
			.mods{
				.preMod{
					.atk_ = buff1,
				},
				.teamPreMod{
					.lunarCharged{
						.DMG = buff2,
					},
				},
			},
			.opts{
				Option::Boolean{
					.key = "fracturedHaloCond1",
					.name = "After an Elemental Skill or Elemental Burst is used",
					.mods{
						.preMod{
							.atk_ = buff1,
						},
					},
				},
				Option::Boolean{
					.key = "fracturedHaloCond2",
					.name = "Equipping character creates a Shield",
					.displayCondition = cond1,
					.mods{
						.teamPreMod{
							.lunarCharged{
								.DMG = buff2,
							},
						},
					},
				},
			},
		};
	},
};
