#include "StaffOfTheScarletSands.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::staffOfTheScarletSands{
	.key{13511},
	.name = "Staff of the Scarlet Sands",
	.baseStats{
		.type = Misc::WeaponType::polearm,
		.rarity = 5,
		.baseAtk = 44.3358,
		.atkCurve = Curves::WeaponGrow::ATTACK_304,
		.subStat = SubStat{
			.stat{
				.stat = Stat::cr,
				.value = 0.096,
			},
			.curve = Curves::WeaponGrow::CRITICAL_301,
		},
		.ascensionUpgrade{0, 31.1, 62.2, 93.4, 124.5, 155.6, 186.7}
	},
	.setup = []() {
		auto eHitsAtk = WeaponMultiplier(Modifiers::total.em, {0.28, 0.35, 0.42, 0.49, 0.56})
					  * GetFloat("eHits");

		return Data::Setup{
			.mods{
				.preMod{
					.atk = WeaponMultiplier(Modifiers::total.em, {0.52, 0.65, 0.78, 0.91, 1.04})
						 + eHitsAtk,
				},
			},
			.opts{
				Option::ValueList{
					.key = "eHits",
					.prefix = "Elemental Skill hits",
					.values{1, 2, 3},
					.mods{
						.preMod{
							.atk = eHitsAtk,
						},
					},
				},
			},
		};
	},
};
