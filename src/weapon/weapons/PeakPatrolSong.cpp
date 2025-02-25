#include "PeakPatrolSong.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::peakPatrolSong{
	.key{11516},
	.name = "Peak Patrol Song",
	.baseStats{
		.type = Misc::WeaponType::sword,
		.rarity = 5,
		.baseAtk = 44.336,
		.atkCurve = Curves::WeaponGrow::ATTACK_304,
		.subStat = SubStat{
			.stat{
				.stat = Stat::def_,
				.value = 0.180,
			},
			.curve = Curves::WeaponGrow::CRITICAL_301,
		},
		.ascensionUpgrade{0.0, 31.1, 62.2, 93.4, 124.5, 155.6, 186.7}
	},
	.setup = []() {
		auto multiplier1 = WeaponMultiplier(true, {0.0800, 0.1000, 0.1200, 0.1400, 0.1600});
		auto multiplier4 = WeaponMultiplier(true, {0.1000, 0.1250, 0.1500, 0.1750, 0.2000});
		auto multiplier5 = WeaponMultiplier(true, {0.0800, 0.1000, 0.1200, 0.1400, 0.1600});

		auto stacks = GetInt("peakPatrolOdeToFlowers");
		auto defBuff = multiplier1 * stacks;
		auto elemBuff = multiplier4 * stacks;
		auto teamElemBuff = Requires(stacks == 2, multiplier5 * (Min(total.def, 3200.f) / Constant(1000.f)));

		return Data::Setup{
			.mods{
				.preMod{
					.def_ = defBuff,
					.allElemental{.DMG = elemBuff},
				},
				.teamPreMod{
					.allElemental{.DMG = teamElemBuff},
				},
			},
			.opts{
				Option::ValueList{
					.key = "peakPatrolOdeToFlowers",
					.prefix = "After Normal or Plunging Attacks hit an opponent",
					.teamBuff = true,
					.values{1, 2},
					.mods{
						.preMod{
							.def_ = defBuff,
							.allElemental{.DMG = elemBuff},
						},
						.teamPreMod{
							.allElemental{.DMG = teamElemBuff},
						},
					},
				},
			},
		};
	},
};
