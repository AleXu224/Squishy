#include "HailstormShrine.hpp"

#include "engine/setup.hpp"

const Engine::Data Engine::Datas::hailstormShrine{
	.key{14109},
	.goodKey{"HailstormShrine"},
	.name = "Hailstorm Shrine",
	.baseStats{
		.specialty = Misc::Specialty::anomaly,
		.rarity = 5,
		.baseAtk = 50,
		.subStat{
			.stat = Stat::cr,
			.value = 0.096,
		},
	},
	.setup = []() {
		auto multiplier1 = EngineMultiplier(true, {0.5f, 0.57f, 0.65f, 0.72f, 0.8f});
		auto multiplier2 = EngineMultiplier(true, {0.2f, 0.23f, 0.26f, 0.29f, 0.32f});

		auto stacks = GetFloat("hailstormShrineStacks");
		auto buff = stacks * multiplier2;

		return Data::Setup{
			.mods{
				.combat{
					.cd = multiplier1,
					.ice{
						.DMG = buff,
					},
				},
			},
			.opts{
				Option::ValueSlider{
					.key = "hailstormShrineStacks",
					.name = "After using an EX Special Attack or after squad member applies an Attribute Anomaly to an enemy",
					.values{0.f, 1.f, 2.f},
					.mods{
						.combat{
							.ice{
								.DMG = buff,
							},
						},
					},
				},
			},
		};
	},
};
