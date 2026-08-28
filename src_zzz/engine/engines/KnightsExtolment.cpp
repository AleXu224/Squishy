#include "KnightsExtolment.hpp"

#include "engine/setup.hpp"

const Engine::Data Engine::Datas::knightsExtolment{
	.key{14159},
	.goodKey{"KnightsExtolment"},
	.name = "Knight's Extolment",
	.baseStats{
		.specialty = Misc::Specialty::attack,
		.rarity = 5,
		.baseAtk = 48,
		.subStat{
			.stat = Stat::cd,
			.value = 0.192,
		},
	},
	.setup = []() {
		auto multiplier1 = EngineMultiplier(true, {0.32f, 0.368f, 0.41600000000000004f, 0.46399999999999997f, 0.512f});
		auto multiplier2 = EngineMultiplier(true, {0.2f, 0.23f, 0.26f, 0.29f, 0.32f});

		auto stacks = GetFloat("knightsExtolmentStacks");
		auto buff = stacks * multiplier1;

		auto fullStackBuff = Requires{
			.requirement = stacks >= 2,
			.ret = 0.f - multiplier2,
		};

		return Data::Setup{
			.mods{
				.combat{
					.cd = buff,
				},
				.enemy{
					.resistance{
						.ice = fullStackBuff,
					},
				},
			},
			.opts{
				Option::ValueSlider{
					.key = "knightsExtolmentStacks",
					.name = "After the equipper's Basic Attack or EX Special Attack's heavy attack hits an enemy",
					.values{0, 1, 2},
					.mods{
						.combat{
							.cd = buff,
						},
						.enemy{
							.resistance{
								.ice = fullStackBuff,
							},
						},
					},
				},
			},
		};
	},
};
