#include "SymphonistOfScents.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::symphonistOfScents{
	.key{13514},
	.goodKey{"SymphonistOfScents"},
	.name = "Symphonist of Scents",
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
		auto multiplier1 = WeaponMultiplier(false, {0.1200, 0.1500, 0.1800, 0.2100, 0.2400});
		auto multiplier2 = WeaponMultiplier(false, {0.1200, 0.1500, 0.1800, 0.2100, 0.2400});
		auto multiplier3 = WeaponMultiplier(false, {0.3200, 0.4000, 0.4800, 0.5600, 0.6400});

		auto offFieldCond = IsActive("symphonistOfScentsOffField");
		auto offFieldBuff = Requires(offFieldCond, multiplier2);

		auto healCond = IsActive("symphonistOfScentsHeal");
		auto healBuff = Requires(healCond, multiplier3);

		return Data::Setup{
			.mods{
				.preMod{
					.atk_ = multiplier1 + offFieldBuff,
				},
				.teamPreMod{
					.atk_ = healBuff,
				},
			},
			.opts{
				Option::Boolean{
					.key = "symphonistOfScentsOffField",
					.name = "Equipping character is off-field",
					.mods{
						.preMod{
							.atk_ = offFieldBuff,
						},
					},
				},
				Option::Boolean{
					.key = "symphonistOfScentsHeal",
					.name = "After healing or being healed by the weapon holder",
					.mods{
						.teamPreMod{
							.atk_ = healBuff,
						},
					},
				},
			},
		};
	},
};
