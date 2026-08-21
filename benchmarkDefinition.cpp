#include "character/characters.hpp"
#include "combo/source.hpp"
#include "optimization/optimizeUpgrade.hpp"
#include "store.hpp"
#include "utils/storageFolder.hpp"
#include "weapon/weapons.hpp"

#include "benchmark/benchmark.h"

#include <memory>

namespace {
	Optimization::UpgradeOptimization &makeOptimization() {
		static std::unique_ptr<Formula::Context> ctx;
		static std::unique_ptr<Optimization::OptionsUpgrade> options;
		static std::unique_ptr<Optimization::UpgradeOptimization> optimization;
		if (optimization) return *optimization;

		Weapon::initWeapons();
		Character::initCharacters();
		Artifact::initArtifacts();
		Store::loadFromFile((Utils::getStorageFolder().value() / "Squishy" / "save.sqsh").string());

		auto &character = Store::characters.at(3);
		auto &team = Store::teams.at(11);
		auto enemy = Store::enemies.insert({
			0,
			Enemy::Instance{
				.key = 0,
				.name = "Default enemy",
			},
		});
		enemy.first->second.stats.sheet.level.constant = 100.f;

		ctx = std::make_unique<Formula::Context>(Formula::Context{
			.source = character.state,
			.origin = character.state,
			.team = team.stats,
			.enemy = enemy.first->second.stats,
		});

		auto node = Combo::Source::Combo{.characterKey = character.instanceKey, .comboKey = 17}.resolve({}).formula;

		options = std::make_unique<Optimization::OptionsUpgrade>(Optimization::OptionsUpgrade{
			.enableLevelUp = false,
			.enableReroll = false,
			.enableDefinition = true,
		});
		for (const auto stat: Stats::Artifact::bySlot(Artifact::Slot::sands)) options->sandsMainStats.emplace(stat, true);
		for (const auto stat: Stats::Artifact::bySlot(Artifact::Slot::goblet)) options->gobletMainStats.emplace(stat, true);
		for (const auto stat: Stats::Artifact::bySlot(Artifact::Slot::circlet)) options->circletMainStats.emplace(stat, true);
		for (const auto stat: Stats::subStats) options->guaranteedSubStats.emplace(stat, true);

		optimization = std::make_unique<Optimization::UpgradeOptimization>(Optimization::UpgradeOptimization{
			.character = character,
			.ctx = *ctx,
			.optimizedNode = node,
			.options = *options,
		});
		return *optimization;
	}

	void definitionOptimization(benchmark::State &state) {
		auto optimization = makeOptimization();
		for (auto _: state) {
			benchmark::DoNotOptimize(optimization.optimize());
		}
	}
}// namespace

BENCHMARK(definitionOptimization);
BENCHMARK_MAIN();
