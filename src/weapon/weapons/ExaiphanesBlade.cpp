#include "ExaiphanesBlade.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::exaiphanesBlade{
	.key{11521},
	.goodKey{"ExaiphanesBlade"},
	.name = "Exaiphanes Blade",
	.baseStats{
		.type = Misc::WeaponType::sword,
		.rarity = 5,
		.baseAtk = 45.936,
		.atkCurve = Curves::WeaponGrow::ATTACK_301,
		.subStat = SubStat{
			.stat{
				.stat = Stat::cr,
				.value = 0.072,
			},
			.curve = Curves::WeaponGrow::CRITICAL_301,
		},
		.ascensionUpgrade{0.0, 31.1, 62.2, 93.4, 124.5, 155.6, 186.7}
	},
	.setup = []() {
		auto multiplier1 = WeaponMultiplier(true, {0.1600, 0.2000, 0.2400, 0.3200, 0.4000});
		auto multiplier3 = WeaponMultiplier(true, {0.0000, 0.0600, 0.0600, 0.0600, 0.0600});

		auto travelerCond = IsActive("exaiphanesBladeTravelerCond");

		auto cond = IsActive("exaiphanesBladeCond");
		auto atkBuff = Requires{
			.requirement = travelerCond && cond,
			.ret = multiplier1,
		};

		auto stacks = GetFloat("exaiphanesBladeStacks");
		auto cdBuff = Requires{
			.requirement = travelerCond,
			.ret = stacks * multiplier3,
		};

		return Data::Setup{
			.mods{
				.preMod{
					.atk_ = atkBuff,
					.cd = cdBuff,
				},
			},
			.opts{
				Option::Boolean{
					.key = "exaiphanesBladeTravelerCond",
					.name = "Equipped by the Traveler",
				},
				Option::Boolean{
					.key = "exaiphanesBladeCond",
					.name = "After hitting an opponent",
					.displayCondition = travelerCond,
					.mods{
						.preMod{
							.atk_ = atkBuff,
						},
					},
				},
				Option::ValueSlider{
					.key = "exaiphanesBladeStacks",
					.name = "Elements the Traveler has resonated with",
					.displayCondition = travelerCond && WeaponRefinement{} > 1,
					.values = std::views::iota(0, 8) | std::ranges::to<std::vector<float>>(),
					.mods{
						.preMod{
							.cd = cdBuff,
						},
					},
				},
			},
		};
	},
};
