#include "KaboomTheCannon.hpp"

#include "engine/setup.hpp"

const Engine::Data Engine::Datas::kaboomTheCannon{
	.key{13115},
	.goodKey{"KaboomTheCannon"},
	.name = "Kaboom the Cannon",
	.baseStats{
		.specialty = Misc::Specialty::support,
		.rarity = 4,
		.baseAtk = 42,
		.subStat{
			.stat = Stat::er_,
			.value = 0.200,
		},
	},
	.setup = []() {
		auto multiplier1 = EngineMultiplier(true, {0.025f, 0.028f, 0.032f, 0.036f, 0.04f});

		auto stacks = GetFloat("kaboomTheCannonStacks");
		auto buff = stacks * multiplier1;

		return Data::Setup{
			.mods{
				.teamCombat{
					.atk_ = buff,
				},
			},
			.opts{
				Option::ValueSlider{
					.key = "kaboomTheCannonStacks",
					.name = "Any friendly unit in the squad attacks and hits an enemy",
					.values = std::views::iota(0, 5) | std::ranges::to<std::vector<float>>(),
					.mods{
						.teamCombat{
							.atk_ = buff,
						},
					},
				},
			},
		};
	},
};
