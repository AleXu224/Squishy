#pragma once

#include "weapon/setup.hpp"

namespace Weapon::Datas {
	const inline Weapon::Data staffOfTheScarletSands{
		.key{13511},
		.name = "Staff of the Scarlet Sands",
		.baseStats{
			.type = Misc::WeaponType::polearm,
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
		.opts{
			Option::ValueList{
				.key = "eHits",
				.prefix = "Elemental Skill hits",
				.values{1, 2, 3},
			},
		},
		.setup = []() {
			return Data::Setup{
				.mods{
					.preMod{
						.atk = WeaponMultiplier(Modifiers::total.em, {0.52, 0.65, 0.78, 0.91, 1.04})
							 + WeaponMultiplier(Modifiers::total.em, {0.28, 0.35, 0.42, 0.49, 0.56})
								   * GetFloat("eHits"),
					},
				},
			};
		},
	};
}// namespace Weapon::Datas
