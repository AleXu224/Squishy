#include "AmenomaKageuchi.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::amenomaKageuchi{
	.key{11414},
	.goodKey{"AmenomaKageuchi"},
	.name = "Amenoma Kageuchi",
	.baseStats{
		.type = Misc::WeaponType::sword,
		.rarity = 4,
		.baseAtk = 41.067,
		.atkCurve = Curves::WeaponGrow::ATTACK_204,
		.subStat = SubStat{
			.stat{
				.stat = Stat::atk_,
				.value = 0.120,
			},
			.curve = Curves::WeaponGrow::CRITICAL_201,
		},
		.ascensionUpgrade{0.0, 25.9, 51.9, 77.8, 103.7, 129.7, 155.6}
	},
	.setup = []() {
		return Data::Setup{};
	},
};
