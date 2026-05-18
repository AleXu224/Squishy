#include "optimizeUpgrade.hpp"

#include "chrono"
#include "execution"
#include "optimization/bnbUpgrade.hpp"
#include "optimization/optimize.hpp"
#include "solutionUpgrade.hpp"
#include "store.hpp"
#include "upgradeFilter.hpp"


Optimization::SolutionsUpgrade Optimization::UpgradeOptimization::optimize() const {
	auto start = std::chrono::high_resolution_clock::now();
	auto prevLoadout = agent.state.loadout().disc.getSlotted();

	std::vector<Disc::Instance> discs;
	discs.reserve(::Store::discs.size());
	for (const auto &[_, disc]: ::Store::discs) {
		if (disc.level >= 15) continue;
		discs.emplace_back(disc);
	}

	std::array<UpgradeFilter, 6> filters;

	for (const auto &[index, partition]: Disc::partitions | std::views::enumerate) {
		auto &filter = filters.at(index);
		filter.partition = partition;
		auto &loadout = agent.state.loadout().disc;
		auto &equippedSlot = loadout.getSlotted().fromPartition(partition);
		if (equippedSlot.key) {
			auto &disc = ::Store::discs.at(equippedSlot.key);
			if (loadout.bonus1.has_value() && loadout.bonus1->setPtr->key == disc.set) {
				filter.set = loadout.bonus1->setPtr->key;
			} else if (loadout.bonus2.has_value() && loadout.bonus2->setPtr->key == disc.set) {
				filter.set = loadout.bonus2->setPtr->key;
			} else if (loadout.bonus3.has_value() && loadout.bonus3->setPtr->key == disc.set) {
				filter.set = loadout.bonus3->setPtr->key;
			}
		}
	}

	auto node = optimizedNode.fold(ctx, {});

	std::atomic<uint64_t> combed = 0;
	static uint64_t runID = 0;
	runID++;
	SolutionsUpgrade solutions;
	auto currentScore = node.eval(ctx);

	for (auto &filter: filters) {
		auto filtered = filter.filter(discs);
		std::for_each(
			std::execution::parallel_unsequenced_policy{},
			filtered.begin(), filtered.end(),
			[&combed, &solutions, &node, &agent_original = agent, discCount = discs.size(), &initialCtx = ctx, runID = runID, currentScore](Disc::Instance &disc) {
				SolutionUpgrade solution{
					.disc = disc.key,
				};
				thread_local uint64_t localRunID = runID;
				thread_local OptimizationThreadData threadData{agent_original, initialCtx};

				if (localRunID != runID) {
					threadData.~OptimizationThreadData();
					new (&threadData) OptimizationThreadData(agent_original, initialCtx);
					localRunID = runID;
				}

				threadData.agent.state.loadout().disc.refreshStats();
				auto lvlsLeft = 15u - disc.level;
				auto rollsLeft = (lvlsLeft) / 3;
				if (lvlsLeft % 3 > 0) rollsLeft++;
				for (auto substat: disc.subStats) {
					if (!substat.activated) {
						if (rollsLeft == 0)
							return;
						rollsLeft--;
					}
				}
				disc.level = 15;
				disc.updateStats();
				if (rollsLeft > 0) {
					BnbUpgrade bnb{
						.disc = disc,
						.agent = threadData.agent,
						.ctx = threadData.ctx,
						.node = node,
						.partition = std::to_underlying(disc.partition),
						.currentScore = currentScore,
					};

					auto agg = bnb.solve({}, rollsLeft);
					solution.upgradeChance = agg.successfulRolls / static_cast<float>(std::pow(4u, rollsLeft));
					solution.upgradeAverage = (agg.upgradeScores / static_cast<double>(agg.successfulRolls)) / currentScore;

					solutions.addSolution(solution);
				}

				combed++;
				std::println("Max chance: {} {}/{} ({}%)", solutions.maxUpgradeChance, combed.load(), discCount, (static_cast<float>(combed) / static_cast<float>(discCount)) * 100.f);
			}
		);
	}

	agent.state.loadout().disc.equipped = prevLoadout;
	agent.state.loadout().disc.refreshStats();

	auto end = std::chrono::high_resolution_clock::now();
	std::println("------------------------------------------------");
	std::println("Optimizing done: Best chance {}, time taken {}", solutions.maxUpgradeChance, std::chrono::duration_cast<std::chrono::milliseconds>(end - start));
	for (const auto &[index, solution]: solutions.solutions | std::views::enumerate) {
		if (!solution.disc) continue;
		std::println("--------------- Disc {}: Chance {}%, Avg Upgrade {}5 -----------------", index, solution.upgradeChance * 100.f, solution.upgradeAverage * 100.f);
		auto &disc = ::Store::discs.at(solution.disc);
		std::println("Main stat: {}, Partition: {}, Set: {}", Utils::Stringify(disc.mainStat), Utils::Stringify(disc.partition), Disc::sets.at(disc.set).name);
		for (const auto &subStat: disc.subStats) {
			if (!subStat.stat.has_value()) continue;
			std::println(
				"Sub stat: {} {}",
				Formula::Percentage(Utils::Stringify(subStat.stat.value()), subStat.value, Utils::isPercentage(subStat.stat.value())),
				subStat.activated ? "" : "(Inactive)"
			);
		}
	}

	return solutions;
}
