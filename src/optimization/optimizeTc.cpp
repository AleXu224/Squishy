#include "optimizeTc.hpp"

#include "chrono"
#include "optimization/bnbTc.hpp"
#include "theorycraftFilter.hpp"


Optimization::SolutionTC Optimization::TCOptimization::optimize() const {
	auto start = std::chrono::high_resolution_clock::now();
	auto prevLoadout = character.state.loadout().artifact.getTheorycraft();

	auto compiledNode = optimizedNode.compile(ctx);

	SolutionTC solution{
		.artifacts = prevLoadout,
		.score = 0,
	};

	RollsValues rolls{};
	TheorycraftFilter filter{};

	auto &equipped = character.state.loadout().artifact.getTheorycraft();
	for (const auto &stat: Stats::subStats) {
		equipped.fromStat(stat) = 0;
	}
	equipped.updateStats();
	character.state.loadout().artifact.refreshStats();
	auto node = optimizedNode.compile(ctx);

	bnbTc(filter, solution, character, rolls, ctx, node, {});

	std::println("Max dmg: {}", solution.score);
	for (const auto &stat: Stats::subStats) {
		std::println("{}: {} rolls", Utils::Stringify(stat), solution.artifacts.fromStat(stat));
	}

	equipped.updateStats();
	character.state.loadout().artifact.equipped = prevLoadout;
	character.state.loadout().artifact.refreshStats();

	auto end = std::chrono::high_resolution_clock::now();
	std::println("------------------------------------------------");
	std::println("Optimizing done: Best dmg {}, time taken {}", solution.score, std::chrono::duration_cast<std::chrono::milliseconds>(end - start));

	return solution;
}
