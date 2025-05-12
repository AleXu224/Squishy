#include "Azurelight.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::azurelight{
	.key{11517},
	.goodKey{"Azurelight"},
	.name = "Azurelight",
	.baseStats{
		.type = Misc::WeaponType::sword,
		.rarity = 5,
		.baseAtk = 47.537,
		.atkCurve = Curves::WeaponGrow::ATTACK_302,
		.subStat = SubStat{
			.stat{
				.stat = Stat::cr,
				.value = 0.048,
			},
			.curve = Curves::WeaponGrow::CRITICAL_301,
		},
		.ascensionUpgrade{0.0, 31.1, 62.2, 93.4, 124.5, 155.6, 186.7}
	},
	.setup = []() {
		auto multiplier1 = WeaponMultiplier(true, {0.2400, 0.3000, 0.3600, 0.4200, 0.4800});
		auto multiplier3 = WeaponMultiplier(true, {0.2400, 0.3000, 0.3600, 0.4200, 0.4800});
		auto multiplier4 = WeaponMultiplier(true, {0.4000, 0.5000, 0.6000, 0.7000, 0.8000});

		auto skillUsedCond = IsActive("azurelightSkillUsed");
		auto skillUsedBuff = Requires(skillUsedCond, multiplier1);

		auto energyCond = IsActive("azurelightEnergyCond");
		auto energyAtkBuff = Requires(skillUsedCond && energyCond, multiplier3);
		auto energyCdBuff = Requires(skillUsedCond && energyCond, multiplier4);

		return Data::Setup{
			.mods{
				.preMod{
					.atk_ = skillUsedBuff + energyAtkBuff,
					.cd = energyCdBuff,
				},
			},
			.opts{
				Option::Boolean{
					.key = "azurelightSkillUsed",
					.name = "After an Elemental Skill is used",
					.mods{
						.preMod{
							.atk_ = skillUsedBuff,
						},
					},
				},
				Option::Boolean{
					.key = "azurelightEnergyCond",
					.name = "Character has 0 energy",
					.displayCondition = skillUsedCond,
					.mods{
						.preMod{
							.atk_ = energyAtkBuff,
							.cd = energyCdBuff,
						},
					},
				},
			},
		};
	},
};
