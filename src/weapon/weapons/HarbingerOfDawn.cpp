#include "HarbingerOfDawn.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::harbingerOfDawn{
	.key{11302},
	.goodKey{"HarbingerOfDawn"},
	.name = "Harbinger of Dawn",
	.baseStats{
		.type = Misc::WeaponType::sword,
		.rarity = 3,
		.baseAtk = 38.741,
		.atkCurve = Curves::WeaponGrow::ATTACK_101,
		.subStat = SubStat{
			.stat{
				.stat = Stat::cd,
				.value = 0.102,
			},
			.curve = Curves::WeaponGrow::CRITICAL_101,
		},
		.ascensionUpgrade{0.0, 19.5, 38.9, 58.4, 77.8, 97.3, 116.7}
	},
	.setup = []() {
		auto multiplier1 = WeaponMultiplier(false, {0.1400, 0.1750, 0.2100, 0.2450, 0.2800});

		auto cond = IsActive("harbingerOfDawnCond");
		auto buff = Requires{
			.requirement = cond,
			.ret = multiplier1,
		};

		return Data::Setup{
			.mods{
				.preMod{
					.cr = buff,
				},
			},
			.opts{
				Option::Boolean{
					.key = "harbingerOfDawnCond",
					.name = "HP is above 90%",
					.mods{
						.preMod{
							.cr = buff,
						},
					},
				},
			},
		};
	},
};
