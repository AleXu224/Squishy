#include "PracticedPerfection.hpp"

#include "engine/setup.hpp"

const Engine::Data Engine::Datas::practicedPerfection{
	.key{14140},
	.goodKey{"PracticedPerfection"},
	.name = "Practiced Perfection",
	.baseStats{
		.specialty = Misc::Specialty::anomaly,
		.rarity = 5,
		.baseAtk = 48,
		.subStat{
			.stat = Stat::atk_,
			.value = 0.120,
		},
	},
	.setup = []() {
		auto multiplier1 = EngineMultiplier(false, {60.f, 69.f, 78.f, 87.f, 96.f});
		auto multiplier2 = EngineMultiplier(true, {0.2f, 0.23f, 0.26f, 0.29f, 0.32f});

		auto cond = IsActive("practicedPerfectionCond");
		auto buff = Requires{
			.requirement = cond,
			.ret = multiplier2 * GetFloat("practicedPerfectionCond"),
		};

		return Data::Setup{
			.mods{
				.combat{
					.am = multiplier1,
					.physical{
						.DMG = buff,
					},
				},
			},
			.opts{
				Option::ValueList{
					.key = "practicedPerfectionCond",
					.prefix = "After inflicting Assault",
					.teamBuff = true,
					.values{1, 2},
					.mods{
						.combat{
							.physical{
								.DMG = buff,
							},
						},
					},
				},
			},
		};
	},
};
