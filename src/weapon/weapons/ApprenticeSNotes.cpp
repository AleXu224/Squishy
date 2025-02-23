#include "ApprenticeSNotes.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::apprenticeSNotes{
	.key{14101},
	.name = "Apprentice's Notes",
	.baseStats{
		.type = Misc::WeaponType::catalyst,
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
