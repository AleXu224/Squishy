#include "FusionCompiler.hpp"

#include "engine/setup.hpp"

const Engine::Data Engine::Datas::fusionCompiler{
	.key{14118},
	.goodKey{"FusionCompiler"},
	.name = "Fusion Compiler",
	.baseStats{
		.specialty = Misc::Specialty::anomaly,
		.rarity = 5,
		.baseAtk = 46,
		.subStat{
			.stat = Stat::penRatio,
			.value = 0.096,
		},
	},
	.setup = []() {
		auto multiplier1 = EngineMultiplier(true, {0.12f, 0.15f, 0.18f, 0.21f, 0.24f});
		auto multiplier2 = EngineMultiplier(false, {25.f, 31.f, 37.f, 43.f, 50.f});

		auto stacks = GetFloat("fusionCompilerStacks");
		auto buff = multiplier2 * stacks;

		return Data::Setup{
			.mods{
				.combat{
					.atk_ = multiplier1,
					.ap = buff,
				},
			},
			.opts{
				Option::ValueSlider{
					.key = "fusionCompilerStacks",
					.name = "After using a Special Attack or EX Special Attack",
					.values = std::views::iota(0, 4) | std::ranges::to<std::vector<float>>(),
					.mods{
						.combat{
							.ap = buff,
						},
					},
				},
			},
		};
	},
};
