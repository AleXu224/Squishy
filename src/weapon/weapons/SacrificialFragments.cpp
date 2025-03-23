#include "SacrificialFragments.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::sacrificialFragments{
	.key{14403},
	.goodKey{"SacrificialFragments"},
	.name = "Sacrificial Fragments",
	.baseStats{
		.type = Misc::WeaponType::catalyst,
		.rarity = 4,
		.baseAtk = 41.067,
		.atkCurve = Curves::WeaponGrow::ATTACK_204,
		.subStat = SubStat{
			.stat{
				.stat = Stat::em,
				.value = 48.000,
			},
			.curve = Curves::WeaponGrow::CRITICAL_201,
		},
		.ascensionUpgrade{0.0, 25.9, 51.9, 77.8, 103.7, 129.7, 155.6}
	},
	.setup = []() {
		return Data::Setup{};
	},
};
