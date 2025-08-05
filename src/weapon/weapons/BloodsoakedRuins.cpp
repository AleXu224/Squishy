#include "BloodsoakedRuins.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::bloodsoakedRuins{
	.key{13516},
	.goodKey{"BloodsoakedRuins"},
	.name = "Bloodsoaked Ruins",
	.baseStats{
		.type = Misc::WeaponType::polearm,
		.rarity = 5,
		.baseAtk = 47.537,
		.atkCurve = Curves::WeaponGrow::ATTACK_302,
		.subStat = SubStat{
			.stat{
				.stat = Stat::cr,
				.value = 0.048,
			},
			.curve = Curves::WeaponGrow::CRITICAL_301,
		},
		.ascensionUpgrade{0.0, 31.1, 62.2, 93.4, 124.5, 155.6, 186.7}
	},
	.setup = []() {
		auto multiplier2 = WeaponMultiplier(true, {0.3600, 0.4800, 0.6000, 0.7200, 0.8400});
		auto multiplier4 = WeaponMultiplier(true, {0.2800, 0.3500, 0.4200, 0.4900, 0.5600});

		auto condBurst = IsActive("bloodsoakedRuinsCondBurst");
		auto buffBurst = Requires{
			condBurst,
			multiplier2,
		};

		auto condReaction = IsActive("bloodsoakedRuinsCondReaction");
		auto buffReaction = Requires{
			condReaction,
			multiplier4,
		};


		return Data::Setup{
			.mods{
				.preMod{
					.cd = buffReaction,
					.lunarCharged{.DMG = buffBurst},
				},
			},
			.opts{
				Option::Boolean{
					.key = "bloodsoakedRuinsCondBurst",
					.name = "After using Elemental Burst",
					.mods{
						.preMod{
							.lunarCharged{.DMG = buffBurst},
						},
					},
				},
				Option::Boolean{
					.key = "bloodsoakedRuinsCondReaction",
					.name = "After triggering a Lunar-Charged reaction",
					.mods{
						.preMod{
							.cd = buffReaction,
						},
					},
				},
			},
		};
	},
};
