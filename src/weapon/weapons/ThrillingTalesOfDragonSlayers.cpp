#include "ThrillingTalesOfDragonSlayers.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::thrillingTalesOfDragonSlayers{
	.key{14302},
	.goodKey{"ThrillingTalesOfDragonSlayers"},
	.name = "Thrilling Tales of Dragon Slayers",
	.baseStats{
		.type = Misc::WeaponType::catalyst,
		.rarity = 3,
		.baseAtk = 38.741,
		.atkCurve = Curves::WeaponGrow::ATTACK_101,
		.subStat = SubStat{
			.stat{
				.stat = Stat::hp_,
				.value = 0.077,
			},
			.curve = Curves::WeaponGrow::CRITICAL_101,
		},
		.ascensionUpgrade{0.0, 19.5, 38.9, 58.4, 77.8, 97.3, 116.7}
	},
	.setup = []() {
		auto multiplier1 = WeaponMultiplier(true, {0.2400, 0.3000, 0.3600, 0.4200, 0.4800});

		auto cond = IsActive("thrillingTalesCond");
		auto buff = Requires(cond, multiplier1);

		return Data::Setup{
			.mods{
				.teamPreMod{
					.atk_ = buff,
				},
			},
			.opts{
				Option::Boolean{
					.key = "thrillingTalesCond",
					.name = "After switching characters",
					.mods{
						.teamPreMod{
							.atk_ = buff,
						},
					},
				},
			},
		};
	},
};
