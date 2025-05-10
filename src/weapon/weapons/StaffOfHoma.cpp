#include "StaffOfHoma.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::staffOfHoma{
	.key{13501},
	.goodKey{"StaffOfHoma"},
	.name = "Staff of Homa",
	.baseStats{
		.type = Misc::WeaponType::polearm,
		.rarity = 5,
		.baseAtk = 45.936,
		.atkCurve = Curves::WeaponGrow::ATTACK_301,
		.subStat = SubStat{
			.stat{
				.stat = Stat::cd,
				.value = 0.144,
			},
			.curve = Curves::WeaponGrow::CRITICAL_301,
		},
		.ascensionUpgrade{0.0, 31.1, 62.2, 93.4, 124.5, 155.6, 186.7}
	},
	.setup = []() {
		auto multiplier1 = WeaponMultiplier(true, {0.2000, 0.2500, 0.3000, 0.3500, 0.4000});
		auto multiplier2 = WeaponMultiplier(true, {0.0080, 0.0100, 0.0120, 0.0140, 0.0160});
		auto multiplier3 = WeaponMultiplier(true, {0.0100, 0.0120, 0.0140, 0.0160, 0.0180});

		auto cond = IsActive("staffOfHomaCond");
		auto buff = Requires{
			cond,
			multiplier3 * total.hp,
		};

		return Data::Setup{
			.mods{
				.preMod{
					.hp_ = multiplier1,
					.atk = multiplier2 * total.hp + buff,
				},
			},
			.opts{
				Option::Boolean{
					.key = "staffOfHomaCond",
					.name = "Wielder's HP is less than 50%",
					.mods{
						.preMod{.atk = buff},
					},
				},
			},
		};
	},
};
