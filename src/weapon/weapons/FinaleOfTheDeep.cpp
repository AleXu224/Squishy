#include "FinaleOfTheDeep.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::finaleOfTheDeep{
	.key{11425},
	.goodKey{"FinaleOfTheDeep"},
	.name = "Finale of the Deep",
	.baseStats{
		.type = Misc::WeaponType::sword,
		.rarity = 4,
		.baseAtk = 43.735,
		.atkCurve = Curves::WeaponGrow::ATTACK_202,
		.subStat = SubStat{
			.stat{
				.stat = Stat::atk_,
				.value = 0.060,
			},
			.curve = Curves::WeaponGrow::CRITICAL_201,
		},
		.ascensionUpgrade{0.0, 25.9, 51.9, 77.8, 103.7, 129.7, 155.6}
	},
	.setup = []() {
		auto multiplier1 = WeaponMultiplier(true, {0.1200, 0.1500, 0.1800, 0.2100, 0.2400});
		auto multiplier4 = WeaponMultiplier(true, {0.0240, 0.0300, 0.0360, 0.0420, 0.0480});
		auto multiplier5 = WeaponMultiplier(false, {150.0000, 187.5000, 225.0000, 262.5000, 300.0000});

		auto cond = IsActive("finaleOfTheDeepCond");
		auto buff = Requires{
			.requirement = cond,
			.ret = multiplier1,
		};

		auto bondOfLifeStacks = GetFloat("finaleOfTheDeepBondOfLifeStacks");
		auto bondOfLifeBuff = Min{
			.val1 = bondOfLifeStacks / ConstantFlat{.value = 100.f} * total.hp * multiplier4,
			.val2 = multiplier5,
		};

		return Data::Setup{
			.mods{
				.preMod{
					.atk = bondOfLifeBuff,
					.atk_ = buff,
				},
			},
			.opts{
				Option::Boolean{
					.key = "finaleOfTheDeepCond",
					.name = "After using Elemental Skill",
					.mods{
						.preMod{
							.atk_ = buff,
						},
					},
				},
				Option::ValueSlider{
					.key = "finaleOfTheDeepBondOfLifeStacks",
					.name = "Bond of Life cleared",
					.values = std::views::iota(0) | std::views::take(26) | std::ranges::to<std::vector<float>>(),
					.mods{
						.preMod{
							.atk = bondOfLifeBuff,
						},
					},
				},
			},
		};
	},
};
