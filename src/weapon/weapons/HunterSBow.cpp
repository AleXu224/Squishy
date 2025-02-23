#include "HunterSBow.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::hunterSBow{
	.key{15101},
	.name = "Hunter's Bow",
	.baseStats{
		.type = Misc::WeaponType::bow,
		.rarity = 1,
		.baseAtk = 23.245,
		.atkCurve = Curves::WeaponGrow::ATTACK_101,
		.subStat{},
		.ascensionUpgrade{0.0, 11.7, 23.3, 35.0, 46.7, 0.0, 0.0}
	},
	.setup = []() {
		return Data::Setup{};
	},
};
