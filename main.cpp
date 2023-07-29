#include "Characters/cyno.hpp"
#include "UI/CharacterCard.hpp"
#include "UI/Masonry.hpp"
#include "UI/NodeCard.hpp"
#include "UI/Screens/CharacterScreen.hpp"
#include "UI/TopNav.hpp"
#include "benchmark/benchmark.h"
#include "box.hpp"
#include "character.hpp"
#include "characterStore.hpp"
#include "node.hpp"
#include "scrollableFrame.hpp"
#include "stat.hpp"
#include "statSheet.hpp"
#include "text.hpp"
#include "window.hpp"
#include <iostream>
#include <memory>
#include <random>

// static void BM_statSheet(benchmark::State &state) {
// 	using namespace Squishy;

// 	// StatSheet::PreMod premod;
// 	StatSheet statSheet{
// 		.level = 90,
// 		.ascension = 6,
// 	};
// 	Cyno.addStatSheetData(statSheet);
// 	statSheet.stats.HP += 1000.0f;
// 	StatSheet statSheet2;
// 	statSheet2.stats.HP += 2000.0f;

// 	statSheet.stats.HP += [](const StatSheet &stats) {
// 		return stats.stats.EM.get(stats) * 0.1f + 100.0f;
// 	};
// 	statSheet.stats.HP += [](const StatSheet &stats) {
// 		return stats.stats.HPPercent.get(stats) * 0.1f + 100.0f;
// 	};
// 	statSheet.stats.HPPercent += [](const StatSheet &stats) {
// 		return stats.stats.EM.get(stats) * 0.1f + 100.0f;
// 	};
// 	float val = 0.0f;

// 	DmgNode node{
// 		.name = "Test",
// 		.statMultipliers = {
// 			DmgNode::StatMultiplier{
// 				.stat = AbilityScalingStat::HP,
// 				.multiplier = 2.35f,
// 			},
// 		},
// 	};
// 	const auto formula = node.bakeFormula(statSheet);

// 	for (auto _: state) {
// 		// memset(&stats, 0, sizeof(StatSheet::stats));
// 		// benchmark::DoNotOptimize(stats);

// 		// StatSheet
// 		// val = statSheet.stats.HP.get(statSheet) + statSheet2.stats.HP.get(statSheet2);
// 		// val = statSheet2.stats.HP.get(statSheet2);
// 		val = formula(statSheet);
// 		// statSheet.stats.clearArtifacts();
// 		// val = node.calculate(statSheet);
// 		// statSheet.stats.clear();

// 		benchmark::DoNotOptimize(statSheet);
// 		benchmark::DoNotOptimize(statSheet2);
// 		benchmark::DoNotOptimize(val);
// 		benchmark::DoNotOptimize(node);
// 	}

// 	// printf("val: %f\n", val);
// }

// BENCHMARK(BM_statSheet);

// BENCHMARK_MAIN();

int main(int, char **) {
	using namespace squi;
	using namespace Squishy;

	for (const auto &charData: CharacterStore::charactersData) {
		CharacterStore::characters.emplace_back(std::make_unique<Character>(charData.second));
	}

	Window window;
	// window.addChild(CharacterScreen{
	// 	.character = *CharacterStore::characters[0],
	// });
	window.addChild(TopNav{
		.tabs{
			{"Characters", CharacterScreen{}},
		},
	});
	window.run();
}
