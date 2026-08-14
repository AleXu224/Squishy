#include "WolfFang.hpp"

#include "weapon/setup.hpp"

const Weapon::Data Weapon::Datas::wolfFang{
	.key{11424},
	.goodKey{"WolfFang"},
	.name = "Wolf-Fang",
	.baseStats{
		.type = Misc::WeaponType::sword,
		.rarity = 4,
		.baseAtk = 42.401,
		.atkCurve = Curves::WeaponGrow::ATTACK_201,
		.subStat = SubStat{
			.stat{
				.stat = Stat::cr,
				.value = 0.060,
			},
			.curve = Curves::WeaponGrow::CRITICAL_201,
		},
		.ascensionUpgrade{0.0, 25.9, 51.9, 77.8, 103.7, 129.7, 155.6}
	},
	.setup = []() {
		auto multiplier1 = WeaponMultiplier(true, {0.1600, 0.2000, 0.2400, 0.2800, 0.3200});
		auto multiplier4 = WeaponMultiplier(true, {0.0200, 0.0250, 0.0300, 0.0350, 0.0400});
		auto multiplier7 = WeaponMultiplier(false, {0.0200, 0.0250, 0.0300, 0.0350, 0.0400});

		auto skillStacks = GetFloat("wolfFangSkillStacks");
		auto skillBuff = multiplier4 * skillStacks;

		auto burstStacks = GetFloat("wolfFangBurstStacks");
		auto burstBuff = multiplier7 * burstStacks;

		return Data::Setup{
			.mods{
				.preMod{
					.skill{
						.DMG = multiplier1,
						.critRate = skillBuff,
					},
					.burst{
						.DMG = multiplier1,
						.critRate = burstBuff,
					},
				},
			},
			.opts{
				Option::ValueSlider{
					.key = "wolfFangSkillStacks",
					.name = "Elemental Skill hits",
					.values = std::views::iota(0) | std::views::take(5) | std::ranges::to<std::vector<float>>(),
					.mods{
						.preMod{
							.skill{
								.critRate = skillBuff,
							},
						},
					},
				},
				Option::ValueSlider{
					.key = "wolfFangBurstStacks",
					.name = "Elemental Burst hits",
					.values = std::views::iota(0) | std::views::take(5) | std::ranges::to<std::vector<float>>(),
					.mods{
						.preMod{
							.burst{
								.critRate = burstBuff,
							},
						},
					},
				},
			},
		};
	},
};
