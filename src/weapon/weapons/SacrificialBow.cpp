#include "SacrificialBow.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::sacrificialBow{
	.key{15403},
	.goodKey{"SacrificialBow"},
	.name = "Sacrificial Bow",
	.baseStats{
		.type = Misc::WeaponType::bow,
		.rarity = 4,
		.baseAtk = 43.735,
		.atkCurve = Curves::WeaponGrow::ATTACK_202,
		.subStat = SubStat{
			.stat{
				.stat = Stat::er,
				.value = 0.067,
			},
			.curve = Curves::WeaponGrow::CRITICAL_201,
		},
		.ascensionUpgrade{0.0, 25.9, 51.9, 77.8, 103.7, 129.7, 155.6}
	},
	.setup = []() {
		return Data::Setup{};
	},
};
