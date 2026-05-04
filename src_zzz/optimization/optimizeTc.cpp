#include "optimizeTc.hpp"

#include "chrono"
#include "optimization/bnbTc.hpp"
#include "theorycraftFilter.hpp"


Optimization::SolutionTC Optimization::TCOptimization::optimize() const {
	auto start = std::chrono::high_resolution_clock::now();
	auto prevLoadout = agent.state.loadout().disc.getTheorycraft();

	auto compiledNode = optimizedNode.fold(ctx, {});

	SolutionTC solution{
		.discs = prevLoadout,
		.score = 0,
	};

	RollsValues rolls{};
	TheorycraftFilter filter{};

	auto &equipped = agent.state.loadout().disc.getTheorycraft();
	for (const auto &stat: Stats::subStats) {
		equipped.fromStat(stat) = 0;
	}
	equipped.updateStats();
	agent.state.loadout().disc.refreshStats();
	auto node = optimizedNode.fold(ctx, {});

	bnbTc(filter, solution, agent, rolls, ctx, node, {});

	std::println("Max dmg: {}", solution.score);
	for (const auto &stat: Stats::subStats) {
		std::println("{}: {} rolls", Utils::Stringify(stat), solution.discs.fromStat(stat));
	}

	equipped.updateStats();
	agent.state.loadout().disc.equipped = prevLoadout;
	agent.state.loadout().disc.refreshStats();

	auto end = std::chrono::high_resolution_clock::now();
	std::println("------------------------------------------------");
	std::println("Optimizing done: Best dmg {}, time taken {}", solution.score, std::chrono::duration_cast<std::chrono::milliseconds>(end - start));

	return solution;
}
