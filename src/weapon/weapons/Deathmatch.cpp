#include "Deathmatch.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::deathmatch{
	.key{13405},
	.goodKey{"Deathmatch"},
	.name = "Deathmatch",
	.baseStats{
		.type = Misc::WeaponType::polearm,
		.rarity = 4,
		.baseAtk = 41.067,
		.atkCurve = Curves::WeaponGrow::ATTACK_204,
		.subStat = SubStat{
			.stat{
				.stat = Stat::cr,
				.value = 0.080,
			},
			.curve = Curves::WeaponGrow::CRITICAL_201,
		},
		.ascensionUpgrade{0.0, 25.9, 51.9, 77.8, 103.7, 129.7, 155.6}
	},
	.setup = []() {
		auto multiplier2 = WeaponMultiplier(true, {0.2400, 0.3000, 0.3600, 0.4200, 0.4800});
		auto multiplier3 = WeaponMultiplier(true, {0.1600, 0.2000, 0.2400, 0.2800, 0.3200});

		auto cond = IsActive("deathmatchCond");
		auto buffAtk = IfElse(cond, multiplier3, multiplier2);
		auto buffDef = Requires(cond, multiplier3);

		return Data::Setup{
			.mods{
				.preMod{
					.atk_ = buffAtk,
					.def_ = buffDef,
				},
			},
			.opts{
				Option::Boolean{
					.key = "deathmatchCond",
					.name = "At least 2 opponents nearby",
					.mods{
						.preMod{
							.atk_ = buffAtk,
							.def_ = buffDef,
						},
					},
				},
			},
		};
	},
};
