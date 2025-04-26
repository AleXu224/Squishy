#include "AquilaFavonia.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::aquilaFavonia{
	.key{11501},
	.goodKey{"AquilaFavonia"},
	.name = "Aquila Favonia",
	.baseStats{
		.type = Misc::WeaponType::sword,
		.rarity = 5,
		.baseAtk = 47.537,
		.atkCurve = Curves::WeaponGrow::ATTACK_302,
		.subStat = SubStat{
			.stat{
				.stat = Stat::physicalDmg,
				.value = 0.090,
			},
			.curve = Curves::WeaponGrow::CRITICAL_301,
		},
		.ascensionUpgrade{0.0, 31.1, 62.2, 93.4, 124.5, 155.6, 186.7}
	},
	.setup = []() {
		auto multiplier1 = WeaponMultiplier(true, {0.2000, 0.2500, 0.3000, 0.3500, 0.4000});
		auto multiplier2 = WeaponMultiplier(false, {1.0000, 1.1500, 1.3000, 1.4500, 1.6000});
		auto multiplier3 = WeaponMultiplier(false, {2.0000, 2.3000, 2.6000, 2.9000, 3.2000});
		return Data::Setup{
			.mods{
				.preMod{
					.atk_ = multiplier1,
				},
			},
			.nodes{
				Node::Heal{
					.name = "Healing",
					.formula = multiplier2 * total.atk,
				},
				Node::CustomAtk{
					.name = "DMG",
					.formula = multiplier3 * total.atk,
				},
			},
		};
	},
};
