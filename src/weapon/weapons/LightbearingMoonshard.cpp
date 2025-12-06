#include "LightbearingMoonshard.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::lightbearingMoonshard{
	.key{11519},
	.goodKey{"LightbearingMoonshard"},
	.name = "Lightbearing Moonshard",
	.baseStats{
		.type = Misc::WeaponType::sword,
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
		auto multiplier1 = WeaponMultiplier(false, {0.6400, 0.8000, 0.9600, 1.1200, 1.2800});
		auto multiplier3 = WeaponMultiplier(false, {0.2000, 0.2500, 0.3000, 0.3500, 0.4000});

		auto cond = IsActive("lightbearingMoonshardCond");
		auto buff = Requires{
			.requirement = cond,
			.ret = multiplier1,
		};

		return Data::Setup{
			.mods{
				.preMod{
					.def_ = multiplier3,
					.lunarCrystallize{.DMG = buff},
				},
			},
			.opts{
				Option::Boolean{
					.key = "lightbearingMoonshardCond",
					.name = "After using Elemental Skill",
					.mods{
						.preMod{
							.lunarCrystallize{.DMG = buff},
						},
					},
				},
			},
		};
	},
};
