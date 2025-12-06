#include "NocturnesCurtainCall.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::nocturnesCurtainCall{
	.key{14522},
	.goodKey{"NocturnesCurtainCall"},
	.name = "Nocturne's Curtain Call",
	.baseStats{
		.type = Misc::WeaponType::catalyst,
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
		auto multiplier1 = WeaponMultiplier(false, {0.1000, 0.1200, 0.1400, 0.1600, 0.1800});
		auto multiplier2 = WeaponMultiplier(false, {0.1400, 0.1600, 0.1800, 0.2000, 0.2200});
		auto multiplier3 = WeaponMultiplier(false, {0.6000, 0.8000, 1.0000, 1.2000, 1.4000});

		auto cond = IsActive("nocturnesCurtainCallCond");
		auto buffHp = Requires{
			.requirement = cond,
			.ret = multiplier2,
		};
		auto buffCritDMG = Requires{
			.requirement = cond,
			.ret = multiplier3,
		};

		return Data::Setup{
			.mods{
				.preMod{
					.hp_ = multiplier1 + buffHp,
					.allLunar{.critDMG = buffCritDMG},
				},
			},
			.opts{
				Option::Boolean{
					.key = "nocturnesCurtainCallCond",
					.name = "After triggering Lunar reactions or inflicting Lunar Reaction DMG",
					.mods{
						.preMod{
							.hp_ = buffHp,
							.allLunar{.critDMG = buffCritDMG},
						},
					},
				},
			},
		};
	},
};
