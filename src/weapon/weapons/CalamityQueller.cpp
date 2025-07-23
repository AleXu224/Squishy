#include "CalamityQueller.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::calamityQueller{
	.key{13507},
	.goodKey{"CalamityQueller"},
	.name = "Calamity Queller",
	.baseStats{
		.type = Misc::WeaponType::polearm,
		.rarity = 5,
		.baseAtk = 49.138,
		.atkCurve = Curves::WeaponGrow::ATTACK_303,
		.subStat = SubStat{
			.stat{
				.stat = Stat::atk_,
				.value = 0.036,
			},
			.curve = Curves::WeaponGrow::CRITICAL_301,
		},
		.ascensionUpgrade{0.0, 31.1, 62.2, 93.4, 124.5, 155.6, 186.7}
	},
	.setup = []() {
		auto multiplier1 = WeaponMultiplier(true, {0.1200, 0.1500, 0.1800, 0.2100, 0.2400});
		auto multiplier3 = WeaponMultiplier(false, {0.0320, 0.0400, 0.0480, 0.0560, 0.0640});

		auto stacks = GetFloat("calamityQuellerStacks");
		auto offField = IsActive("calamityQuellerOffField");

		auto buff = multiplier3
				  * stacks
				  * IfElse{
					  .requirement = offField,
					  .trueVal = Constant(2.f),
					  .elseVal = Constant(1.f),
				  };

		return Data::Setup{
			.mods{
				.preMod{
					.atk_ = buff,
					.allElemental{.DMG = multiplier1},
				},
			},
			.opts{
				Option::ValueList{
					.key = "calamityQuellerStacks",
					.prefix = "After using an Elemental Skill",
					.teamBuff = true,
					.values{1, 2, 3, 4, 5, 6},
					.mods{
						.preMod{
							.atk_ = buff,
						},
					},
				},
				Option::Boolean{
					.key = "calamityQuellerOffField",
					.name = "Character is Off-field",
					.teamBuff = true,
				},
			},
		};
	},
};
