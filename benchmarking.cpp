#include "benchmark/benchmark.h"
#include "Characters/cyno.hpp"

static void BM_statSheet(benchmark::State &state) {
	using namespace Squishy;

	StatSheet statSheet{
		.level = 90,
		.ascension = 6,
	};
	Cyno.addStatSheetData(statSheet);
	statSheet.stats.HP += 1000.0f;
	StatSheet statSheet2;
	statSheet2.stats.HP += 2000.0f;

	statSheet.stats.HP += [](const StatSheet &stats) {
		return stats.stats.EM.get(stats) * 0.1f + 100.0f;
	};
	statSheet.stats.HP += [](const StatSheet &stats) {
		return stats.stats.HPPercent.get(stats) * 0.1f + 100.0f;
	};
	statSheet.stats.HPPercent += [](const StatSheet &stats) {
		return stats.stats.EM.get(stats) * 0.1f + 100.0f;
	};
	float val = 0.0f;

	DmgNode node{
		.name = "Test",
		.statMultipliers = {
			DmgNode::StatMultiplier{
				.stat = AbilityScalingStat::HP,
				.multiplier = 2.35f,
			},
		},
	};
	const auto formula = node.bakeFormula(statSheet);

	for (auto _: state) {
		// memset(&stats, 0, sizeof(StatSheet::stats));
		// benchmark::DoNotOptimize(stats);

		// StatSheet
		// val = statSheet.stats.HP.get(statSheet) + statSheet2.stats.HP.get(statSheet2);
		// val = statSheet2.stats.HP.get(statSheet2);
		val = formula(statSheet);
		// statSheet.stats.clearArtifacts();
		// val = node.calculate(statSheet);
		// statSheet.stats.clear();

		benchmark::DoNotOptimize(statSheet);
		benchmark::DoNotOptimize(statSheet2);
		benchmark::DoNotOptimize(val);
		benchmark::DoNotOptimize(node);
	}

	// printf("val: %f\n", val);
}

BENCHMARK(BM_statSheet);

BENCHMARK_MAIN();