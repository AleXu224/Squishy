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
		return Data::Setup{};
	},
};
