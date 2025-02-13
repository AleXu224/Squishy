#pragma once

#include "character/setup.hpp"

inline Character::Data testCharacter{
	.key{1},
	.name{"Test Character"},
	.baseStats{
		.baseHp = 972.3864,
		.baseAtk = 24.7646,
		.baseDef = 66.88994,
		.ascensionStat = Stat::cd,
		.rarity = 5,
		.c3Talent = ConstellationTalent::burst,
		.c5Talent = ConstellationTalent::skill,
		.element = Misc::Element::electro,
		.weaponType = Misc::WeaponType::polearm,
		.hpCurve = Curves::CharacterGrow::HP_S5,
		.atkCurve = Curves::CharacterGrow::ATTACK_S5,
		.defCurve = Curves::CharacterGrow::HP_S5,
		.hpUpgrade = {0, 833.7334, 1426.1229, 2215.9756, 2808.3652, 3400.7546, 3993.1443},
		.atkUpgrade = {0, 21.231853, 36.317646, 56.432034, 71.51782, 86.603615, 101.68941},
		.defUpgrade = {0, 57.3534, 98.1045, 152.4393, 193.1904, 233.9415, 274.6926},
		.ascensionStatUpgrade = {0, 0, 0.096, 0.192, 0.192, 0.288, 0.384},
	},
	.setup = []() {
		return Character::Data::Setup{};
	},
};