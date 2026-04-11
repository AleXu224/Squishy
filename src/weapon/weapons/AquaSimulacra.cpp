#include "AquaSimulacra.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::aquaSimulacra{
	.key{15508},
	.goodKey{"AquaSimulacra"},
	.name = "Aqua Simulacra",
	.baseStats{
		.type = Misc::WeaponType::bow,
		.rarity = 5,
		.baseAtk = 44.336,
		.atkCurve = Curves::WeaponGrow::ATTACK_304,
		.subStat = SubStat{
			.stat{
				.stat = Stat::cd,
				.value = 0.192,
			},
			.curve = Curves::WeaponGrow::CRITICAL_301,
		},
		.ascensionUpgrade{0.0, 31.1, 62.2, 93.4, 124.5, 155.6, 186.7}
	},
	.setup = []() {
		auto multiplier1 = WeaponMultiplier(true, {0.1600, 0.2000, 0.2400, 0.2800, 0.3200});
		auto multiplier2 = WeaponMultiplier(true, {0.2000, 0.2500, 0.3000, 0.3500, 0.4000});

		auto cond = IsActive("aquaSimulacraCond");
		auto buff = Requires{cond, multiplier2};

		return Data::Setup{
			.mods{
				.preMod{
					.hp_ = multiplier1,
					.all{
						.DMG = buff,
					},
				},
			},
			.opts{
				Option::Boolean{
					.key = "aquaSimulacraCond",
					.name = "Opponents nearby",
					.mods{
						.preMod{
							.all{
								.DMG = buff,
							},
						},
					},
				},
			},
		};
	},
};
