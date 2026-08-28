#include "ChiefSidekick.hpp"

#include "engine/setup.hpp"

const Engine::Data Engine::Datas::chiefSidekick{
	.key{14157},
	.goodKey{"ChiefSidekick"},
	.name = "Chief Sidekick",
	.baseStats{
		.specialty = Misc::Specialty::stun,
		.rarity = 5,
		.baseAtk = 48,
		.subStat{
			.stat = Stat::cr,
			.value = 0.096,
		},
	},
	.setup = []() {
		auto multiplier1 = EngineMultiplier(false, {30.f, 33.f, 36.f, 39.f, 42.f});
		auto multiplier2 = EngineMultiplier(true, {0.15f, 0.172f, 0.195f, 0.217f, 0.24f});
		auto multiplier4 = EngineMultiplier(true, {0.125f, 0.14400000000000002f, 0.163f, 0.18100000000000002f, 0.2f});

		auto stacks = GetFloat("chiefSidekickStacks");
		auto buff = stacks * multiplier4;

		return Data::Setup{
			.mods{
				.combat{
					.impact = multiplier1,
					.all{
						.enemy{
							.resistance{
								.fire = 0.f - multiplier2,
							},
						},
					},
				},
				.teamCombat{
					.all{
						.DMG = buff,
					},
				},
			},
			.opts{
				Option::ValueSlider{
					.key = "chiefSidekickStacks",
					.name = "After the equipper deals Fire DMG with an EX Special Attack",
					.teamBuff = true,
					.values{0, 1, 2},
					.mods{
						.teamCombat{
							.all{
								.DMG = buff,
							},
						},
					},
				},
			},
		};
	},
};
