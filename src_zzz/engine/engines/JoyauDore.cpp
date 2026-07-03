#include "JoyauDore.hpp"

#include "engine/setup.hpp"

const Engine::Data Engine::Datas::joyauDore{
	.key{14156},
	.goodKey{"JoyauDore"},
	.name = "Joyau Dore",
	.baseStats{
		.specialty = Misc::Specialty::anomaly,
		.rarity = 5,
		.baseAtk = 48,
		.subStat{
			.stat = Stat::er_,
			.value = 0.240,
		},
	},
	.setup = []() {
		auto multiplier1 = EngineMultiplier(false, {70.f, 80.f, 90.f, 100.f, 110.f});
		auto multiplier2 = EngineMultiplier(true, {0.07f, 0.08f, 0.09f, 0.1f, 0.11f});
		auto multiplier3 = EngineMultiplier(false, {60.f, 69.f, 78.f, 87.f, 96.f});

		auto exStacks = GetFloat("joyaruDoreExCond");
		auto exBuff = exStacks * multiplier2;

		auto apBuff = Requires{
			.requirement = exStacks >= 2.f,
			.ret = multiplier3,
		};

		return Data::Setup{
			.mods{
				.combat{
					.ap = multiplier1,
					.windswept{
						.DMG = exBuff,
					},
					.vortex{
						.DMG = exBuff,
					},
				},
				.teamCombat{
					.ap = apBuff,
				},
			},
			.opts{
				Option::ValueSlider{
					.key = "joyaruDoreExCond",
					.name = "When the equipper's EX Special Attack deals Wind DMG",
					.teamBuff = true,
					.values{0.f, 1.f, 2.f},
					.mods{
						.combat{
							.windswept{
								.DMG = exBuff,
							},
							.vortex{
								.DMG = exBuff,
							},
						},
						.teamCombat{
							.ap = apBuff,
						},
					},
				},
			},
		};
	},
};
