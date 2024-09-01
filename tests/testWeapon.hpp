#pragma once

#include "weapon/setup.hpp"

inline Weapon::Data testWeapon{
	.key = 2,
	.name = "Test Weapon",
	.baseStats{
		.type = WeaponType::polearm,
		.baseAtk = 44.3358,
		.substat{
			.stat = Stat::cr,
			.value = 0.096,
		},
		.atkCurve = Curves::WeaponGrow::ATTACK_304,
		.substatCurve = Curves::WeaponGrow::CRITICAL_301,
		.ascensionUpgrade{
			0, 31.1, 62.2, 93.4, 124.5, 155.6, 186.7
		}
	},
	.setup = []() {
		return Weapon::Data::Setup{};
	},
};