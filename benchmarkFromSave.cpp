#include "store.hpp"

#include "character/characters.hpp"
#include "utils/storageFolder.hpp"
#include "weapon/weapons.hpp"


#include "benchmark/benchmark.h"

void benchFromSave(benchmark::State &state) {
	Weapon::initWeapons();
	Character::initCharacters();
	Artifact::initArtifacts();

	auto fileSavePath = Utils::getStorageFolder().value() / "Squishy" / "save.sqsh";
	::Store::loadFromFile(fileSavePath.string());

	auto enemy = Store::enemies.insert(
		{
			0,
			Enemy::Instance{
				.key = 0,
				.name = "Default enemy",
			},
		}
	);

	enemy.first->second.stats.sheet.level.constant = 100.f;

	auto &cyno = Store::characters.at(3);
	auto &team = Store::teams.at(11);

	auto ctx = Formula::Context{
		.source = cyno.state,
		.origin = cyno.state,
		.team = team.stats,
		.enemy = enemy.first->second.stats,
	};

	auto source = Combo::Source::Combo{
		.characterKey = cyno.instanceKey,
		.comboKey = 17,
	};

	auto node = source.resolve({}).formula;
	{
		Formula::enableAllocator = true;
		auto folded = node.fold(ctx, {});

		for (auto _: state) {
			auto result = folded.eval(ctx);
			benchmark::DoNotOptimize(result);
		}
	}

	Formula::enableAllocator = false;
	Formula::NodeAllocator::reset();
}

BENCHMARK(benchFromSave);
BENCHMARK_MAIN();