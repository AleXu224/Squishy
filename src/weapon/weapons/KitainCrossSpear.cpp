#include "KitainCrossSpear.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::kitainCrossSpear{
	.key{13414},
	.goodKey{"KitainCrossSpear"},
	.name = "Kitain Cross Spear",
	.baseStats{
		.type = Misc::WeaponType::polearm,
		.rarity = 4,
		.baseAtk = 43.735,
		.atkCurve = Curves::WeaponGrow::ATTACK_202,
		.subStat = SubStat{
			.stat{
				.stat = Stat::em,
				.value = 24.000,
			},
			.curve = Curves::WeaponGrow::CRITICAL_201,
		},
		.ascensionUpgrade{0.0, 25.9, 51.9, 77.8, 103.7, 129.7, 155.6}
	},
	.setup = []() {
		auto multiplier1 = WeaponMultiplier(false, {0.0600, 0.0750, 0.0900, 0.1050, 0.1200});
		return Data::Setup{
			.mods{
				.preMod{
					.skill{
						.DMG = multiplier1,
					},
				},
			},
		};
	},
};
