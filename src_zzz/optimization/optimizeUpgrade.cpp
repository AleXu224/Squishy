#include "optimizeUpgrade.hpp"

#include "chrono"
#include "execution"
#include "formula/substatDependencies.hpp"
#include "optimization/definitionChanceCalculator.hpp"
#include "optimization/optimize.hpp"
#include "solutionUpgrade.hpp"
#include "stats/disc.hpp"
#include "store.hpp"
#include "upgradeFilter.hpp"

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <ranges>
#include <unordered_set>
#include <utility>
#include <vector>


Optimization::SolutionsUpgrade Optimization::UpgradeOptimization::optimize() const {
	auto start = std::chrono::high_resolution_clock::now();
	auto prevLoadout = agent.state.loadout().disc.getSlotted();

	auto mainStatEnabled = [&](const Disc::Instance &disc) {
		switch (disc.partition) {
			case Disc::Partition::four:
				return options.partition4MainStats.at(disc.mainStat);
			case Disc::Partition::five:
				return options.partition5MainStats.at(disc.mainStat);
			case Disc::Partition::six:
				return options.partition6MainStats.at(disc.mainStat);
			default:
				return true;
		}
	};

	std::vector<Disc::Instance> discs;
	discs.reserve(::Store::discs.size());
	for (const auto &[_, disc]: ::Store::discs) {
		if (disc.level >= 15) continue;
		if (!mainStatEnabled(disc)) continue;
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

	std::unordered_set<Stat> substatDependencies;
	Formula::FloatNode node;
	{
		Formula::SubstatDependencyScope scope;
		node = optimizedNode.fold(ctx, {});
		substatDependencies = std::move(scope.dependencies);
	}

	std::atomic<uint64_t> combed = 0;
	static uint64_t runID = 0;
	runID++;
	SolutionsUpgrade solutions;
	auto currentScore = node.eval(ctx);

	if (options.enableLevelUp) {
		for (auto &filter: filters) {
			auto filtered = filter.filter(discs);
			std::for_each(
				std::execution::parallel_unsequenced_policy{},
				filtered.begin(), filtered.end(),
				[&combed, &solutions, &node, &substatDependencies, &agent_original = agent, discCount = discs.size(), &initialCtx = ctx, runID = runID, currentScore](Disc::Instance &disc) {
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

					std::vector<size_t> unfilledIndices;
					for (size_t i = 0; i < disc.subStats.size(); i++) {
						if (!disc.subStats.at(i).stat.has_value()) unfilledIndices.push_back(i);
					}
					auto slotsToFill = std::min(unfilledIndices.size(), static_cast<size_t>(rollsLeft));
					rollsLeft -= static_cast<uint8_t>(slotsToFill);
					if (rollsLeft == 0 && slotsToFill == 0) {
						combed++;
						return;
					}

					std::vector<Stat> pool;
					for (const auto &stat: Stats::Disc::subStats) {
						if (stat == disc.mainStat) continue;
						bool present = false;
						for (const auto &subStat: disc.subStats) {
							if (subStat.stat == stat) {
								present = true;
								break;
							}
						}
						if (!present) pool.push_back(stat);
					}
					if (pool.size() < slotsToFill) {
						combed++;
						return;
					}

					double assignmentCount = 1;
					for (size_t k = 0; k < slotsToFill; k++) {
						assignmentCount *= static_cast<double>(pool.size() - k);
					}

					SolutionUpgrade solution{
						.disc = disc.key,
						.kind = SolutionUpgrade::Kind::levelUp,
					};
					double totalChance = 0;
					double totalUpgradeScores = 0;

					std::vector<Stat> chosen;
					chosen.reserve(slotsToFill);
					auto enumerate = [&](this auto &&self, size_t depth) {
						if (depth == slotsToFill) {
							Disc::Instance leveled = disc;
							leveled.level = 15;
							std::array<uint8_t, 4> baseRolls{};
							for (size_t k = 0; k < slotsToFill; k++) {
								auto idx = unfilledIndices.at(k);
								leveled.subStats.at(idx).stat = chosen.at(k);
								leveled.subStats.at(idx).activated = false;
								baseRolls.at(idx) = 1;
							}
							leveled.updateStats();

							BnbUpgrade bnb{
								.disc = leveled,
								.agent = threadData.agent,
								.ctx = threadData.ctx,
								.node = node,
								.partition = std::to_underlying(disc.partition),
								.currentScore = currentScore,
								.baseRolls = baseRolls,
								.rollsLeft = static_cast<uint8_t>(rollsLeft),
								.substatDependencies = &substatDependencies,
							};
							auto agg = bnb.solve();
							totalChance += agg.chance / assignmentCount;
							totalUpgradeScores += agg.upgradeScores / assignmentCount;
							return;
						}
						for (const auto &stat: pool) {
							bool used = false;
							for (const auto &c: chosen) {
								if (c == stat) {
									used = true;
									break;
								}
							}
							if (used) continue;
							chosen.push_back(stat);
							self(depth + 1);
							chosen.pop_back();
						}
					};
					enumerate(0);

					solution.upgradeChance = static_cast<float>(totalChance);
					solution.upgradeAverage = static_cast<float>(totalChance > 0 ? (totalUpgradeScores / totalChance / currentScore) : 0);
					solution.score = solution.upgradeChance * solution.upgradeAverage;
					if (solution.score > 0.f) solutions.addSolution(solution);

					combed++;
					std::println("Max score: {} {}/{} ({}%)", solutions.maxScore, combed.load(), discCount, (static_cast<float>(combed) / static_cast<float>(discCount)) * 100.f);
				}
			);
		}
	}

	if (options.enableDefinition) {
		std::vector<DefinitionChanceCalculator::Candidate> defCandidates;
		for (const auto &[index, partition]: Disc::partitions | std::views::enumerate) {
			if (!filters.at(index).set) continue;
			auto candidates = DefinitionChanceCalculator::makeCandidates(filters.at(index).set, partition, options);
			defCandidates.insert(defCandidates.end(), candidates.begin(), candidates.end());
		}
		if (!defCandidates.empty()) {
			std::for_each(
				std::execution::parallel_unsequenced_policy{},
				defCandidates.begin(), defCandidates.end(),
				[&solutions, &node, &substatDependencies, &agent_original = agent, &initialCtx = ctx, &options = options, runID = runID, currentScore](DefinitionChanceCalculator::Candidate &candidate) {
					thread_local uint64_t localRunID = runID;
					thread_local OptimizationThreadData threadData{agent_original, initialCtx};

					if (localRunID != runID) {
						threadData.~OptimizationThreadData();
						new (&threadData) OptimizationThreadData(agent_original, initialCtx);
						localRunID = runID;
					}

					threadData.agent.state.loadout().disc.refreshStats();
					DefinitionChanceCalculator calc{
						.agent = threadData.agent,
						.ctx = threadData.ctx,
						.node = node,
						.options = options,
						.currentScore = currentScore,
						.substatDependencies = &substatDependencies,
					};
					solutions.addSolution(calc.exactSolution(candidate));
				}
			);
		}
	}

	agent.state.loadout().disc.equipped = prevLoadout;
	agent.state.loadout().disc.refreshStats();

	auto end = std::chrono::high_resolution_clock::now();
	std::println("------------------------------------------------");
	std::println("Optimizing done: Best score {}, time taken {}", solutions.maxScore, std::chrono::duration_cast<std::chrono::milliseconds>(end - start));
	for (const auto &[index, solution]: solutions.solutions | std::views::enumerate) {
		if (solution.kind == SolutionUpgrade::Kind::definition) {
			std::println("--------------- Definition {}: Chance {}%, Avg Upgrade {}% -----------------", index, solution.upgradeChance * 100.f, solution.upgradeAverage * 100.f);
			std::println("Set: {}, Partition: {}, Main stat: {}", Disc::sets.at(solution.set).name, Utils::Stringify(solution.partition), Utils::Stringify(solution.mainStat));
			if (solution.chosenCount == 0) {
				std::println("No targeted sub stats");
			} else {
				std::println("Chosen sub stats: {} ({} targeted)", Utils::Stringify(solution.chosenSubStats.at(0)), solution.chosenCount);
				if (solution.chosenCount > 1) {
					std::println("                   + {}", Utils::Stringify(solution.chosenSubStats.at(1)));
				}
			}
			continue;
		}
		if (!solution.disc) continue;
		std::println("--------------- Disc {}: Chance {}%, Avg Upgrade {}% -----------------", index, solution.upgradeChance * 100.f, solution.upgradeAverage * 100.f);
		auto &disc = ::Store::discs.at(solution.disc);
		std::println("Main stat: {}, Partition: {}, Set: {}", Utils::Stringify(disc.mainStat), Utils::Stringify(disc.partition), Disc::sets.at(disc.set).name);
		for (const auto &subStat: disc.subStats) {
			if (!subStat.stat.has_value()) continue;
			std::println(
				"Sub stat: {} {}",
				Formula::Percentage(Utils::Stringify(subStat.stat.value()), subStat.getValue(disc.rarity), Utils::isPercentage(subStat.stat.value())),
				subStat.activated ? "" : "(Inactive)"
			);
		}
	}

	return solutions;
}
