#include "TomeOfTheEternalFlow.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::tomeOfTheEternalFlow{
	.key{14514},
	.goodKey{"TomeOfTheEternalFlow"},
	.name = "Tome of the Eternal Flow",
	.baseStats{
		.type = Misc::WeaponType::catalyst,
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
		auto multiplier1 = WeaponMultiplier(true, {0.16000, 0.2000, 0.2400, 0.2800, 0.3200});
		auto multiplier3 = WeaponMultiplier(true, {0.1400, 0.1800, 0.2200, 0.2600, 0.3000});

		auto cond = GetFloat("tomeOfTheEternalFlowCond");
		auto buff = cond * multiplier3;

		return Data::Setup{
			.mods{
				.preMod{
					.hp_ = multiplier1,
					.charged{.DMG = buff},
				},
			},
			.opts{
				Option::ValueList{
					.key = "tomeOfTheEternalFlowCond",
					.prefix = "HP increases or decreases",
					.values{1, 2, 3},
					.mods{
						.preMod{
							.charged{.DMG = buff},
						},
					},
				},
			},
		};
	},
};
