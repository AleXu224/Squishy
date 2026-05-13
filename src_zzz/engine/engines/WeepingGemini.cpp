#include "WeepingGemini.hpp"

#include "engine/setup.hpp"

const Engine::Data Engine::Datas::weepingGemini{
	.key{13008},
	.goodKey{"WeepingGemini"},
	.name = "Weeping Gemini",
	.baseStats{
		.specialty = Misc::Specialty::anomaly,
		.rarity = 4,
		.baseAtk = 40,
		.subStat{
			.stat = Stat::atk_,
			.value = 0.100,
		},
	},
	.setup = []() {
		auto stacks = GetFloat("weepingGeminiCond");
		auto buff = EngineMultiplier(true, {30.f, 34.f, 38.f, 42.f, 48.f}) * stacks;

		return Data::Setup{
			.mods{
				.combat{
					.ap = buff,
				},
			},
			.opts{
				Option::ValueList{
					.key = "weepingGeminiCond",
					.prefix = "A squad member inflicted an Attribute Anomaly on an enemy",
					.teamBuff = true,
					.values{1, 2, 3, 4},
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
