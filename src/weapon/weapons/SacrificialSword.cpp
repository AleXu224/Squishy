#include "SacrificialSword.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::sacrificialSword{
	.key{11403},
	.goodKey{"SacrificialSword"},
	.name = "Sacrificial Sword",
	.baseStats{
		.type = Misc::WeaponType::sword,
		.rarity = 4,
		.baseAtk = 41.067,
		.atkCurve = Curves::WeaponGrow::ATTACK_204,
		.subStat = SubStat{
			.stat{
				.stat = Stat::er,
				.value = 0.133,
			},
			.curve = Curves::WeaponGrow::CRITICAL_201,
		},
		.ascensionUpgrade{0.0, 25.9, 51.9, 77.8, 103.7, 129.7, 155.6}
	},
	.setup = []() {
		return Data::Setup{};
	},
};
