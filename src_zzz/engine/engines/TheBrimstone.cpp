#include "TheBrimstone.hpp"

#include "engine/setup.hpp"

const Engine::Data Engine::Datas::theBrimstone{
	.key{14104},
	.goodKey{"TheBrimstone"},
	.name = "The Brimstone",
	.baseStats{
		.specialty = Misc::Specialty::attack,
		.rarity = 5,
		.baseAtk = 46,
		.subStat{
			.stat = Stat::atk_,
			.value = 0.120,
		},
	},
	.setup = []() {
		auto multiplier1 = EngineMultiplier(true, {0.035f, 0.044000000000000004f, 0.052000000000000005f, 0.06f, 0.07f});

		auto stacks = GetFloat("theBrimstoneStacks");
		auto buff = stacks * multiplier1;

		return Data::Setup{
			.mods{
				.combat{
					.atk_ = buff,
				},
			},
			.opts{
				Option::ValueSlider{
					.key = "theBrimstoneStacks",
					.name = "After hitting an enemy with a Basic Attack, Dash Attack, or Dodge Counter",
					.values = std::views::iota(0) | std::views::take(9) | std::ranges::to<std::vector<float>>(),
				},
			},
		};
	},
};
