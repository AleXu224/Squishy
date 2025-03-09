#pragma once

#include "weapon/setup.hpp"

inline Weapon::Data testWeapon{
	.key{2},
	.goodKey{},
	.name = "Test Weapon",
	.baseStats{
		.type = Misc::WeaponType::polearm,
		.rarity = 5,
		.baseAtk = 44.3358,
		.atkCurve = Curves::WeaponGrow::ATTACK_304,
		.subStat = Weapon::SubStat{
			.stat{
				.stat = Stat::cr,
				.value = 0.096,
			},
			.curve = Curves::WeaponGrow::CRITICAL_301,
		},
		.ascensionUpgrade{0, 31.1, 62.2, 93.4, 124.5, 155.6, 186.7}
	},
	.setup = []() {
		return Weapon::Data::Setup{};
	},
};