#include "FavoniusCodex.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::favoniusCodex{
	.key{14401},
	.goodKey{"FavoniusCodex"},
	.name = "Favonius Codex",
	.baseStats{
		.type = Misc::WeaponType::catalyst,
		.rarity = 4,
		.baseAtk = 42.401,
		.atkCurve = Curves::WeaponGrow::ATTACK_201,
		.subStat = SubStat{
			.stat{
				.stat = Stat::er,
				.value = 0.100,
			},
			.curve = Curves::WeaponGrow::CRITICAL_201,
		},
		.ascensionUpgrade{0.0, 25.9, 51.9, 77.8, 103.7, 129.7, 155.6}
	},
	.setup = []() {
		return Data::Setup{};
	},
};
