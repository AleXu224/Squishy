#include "StarcallersWatch.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::starcallersWatch{
	.key{14517},
	.goodKey{"StarcallersWatch"},
	.name = "Starcaller's Watch",
	.baseStats{
		.type = Misc::WeaponType::catalyst,
		.rarity = 5,
		.baseAtk = 44.336,
		.atkCurve = Curves::WeaponGrow::ATTACK_304,
		.subStat = SubStat{
			.stat{
				.stat = Stat::em,
				.value = 57.600,
			},
			.curve = Curves::WeaponGrow::CRITICAL_301,
		},
		.ascensionUpgrade{0.0, 31.1, 62.2, 93.4, 124.5, 155.6, 186.7}
	},
	.setup = []() {
		auto multiplier1 = WeaponMultiplier(true, {0.2800, 0.3500, 0.4200, 0.4900, 0.5600});
		auto multiplier4 = WeaponMultiplier(false, {100.0000, 125.0000, 150.0000, 175.0000, 200.0000});

		auto cond = IsActive("starcallersWatchCond");
		auto buff = Requires(cond, multiplier1);

		return Data::Setup{
			.mods{
				.preMod{
					.em = multiplier4,
				},
				.teamPreMod{
					.all{.DMG = buff},
				},
			},
			.opts{
				Option::Boolean{
					.key = "starcallersWatchCond",
					.name = "After the equipping character creates a shield",
					.mods{
						.teamPreMod{
							.all{.DMG = buff},
						},
					},
				},
			},
		};
	},
};
