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
	auto prevLoadout = character.state.loadout().artifact.getSlotted();

	std::vector<Artifact::Instance> artifacts;
	artifacts.reserve(::Store::artifacts.size());
	for (const auto &[_, artifact]: ::Store::artifacts) {
		if (artifact.level >= 20) continue;
		artifacts.emplace_back(artifact);
	}

	std::array<UpgradeFilter, 5> filters;

	for (const auto &[index, slot]: Artifact::slots | std::views::enumerate) {
		auto &filter = filters.at(index);
		filter.slot = slot;
		auto &loadout = character.state.loadout().artifact;
		auto &equippedSlot = loadout.getSlotted().fromSlot(slot);
		if (equippedSlot.key) {
			auto &artifact = ::Store::artifacts.at(equippedSlot.key);
			if (loadout.bonus1.has_value() && loadout.bonus1->setPtr->key == artifact.set) {
				filter.set = loadout.bonus1->setPtr->key;
			} else if (loadout.bonus2.has_value() && loadout.bonus2->setPtr->key == artifact.set) {
				filter.set = loadout.bonus2->setPtr->key;
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
		auto filtered = filter.filter(artifacts);
		std::for_each(
			std::execution::parallel_unsequenced_policy{},
			filtered.begin(), filtered.end(),
			[&combed, &solutions, &node, &character_original = character, artifactCount = artifacts.size(), &initialCtx = ctx, runID = runID, currentScore](Artifact::Instance &artifact) {
				SolutionUpgrade solution{
					.artifact = artifact.key,
				};
				thread_local uint64_t localRunID = runID;
				thread_local OptimizationThreadData threadData{character_original, initialCtx};

				if (localRunID != runID) {
					threadData.~OptimizationThreadData();
					new (&threadData) OptimizationThreadData(character_original, initialCtx);
					localRunID = runID;
				}

				threadData.character.state.loadout().artifact.refreshStats();
				auto lvlsLeft = 20u - artifact.level;
				auto rollsLeft = (lvlsLeft) / 4;
				if (lvlsLeft % 4 > 0) rollsLeft++;
				for (auto substat: artifact.subStats) {
					if (!substat.activated) {
						if (rollsLeft == 0)
							return;
						rollsLeft--;
					}
				}
				artifact.level = 20;
				artifact.updateStats();
				if (rollsLeft > 0) {
					BnbUpgrade bnb{
						.artifact = artifact,
						.character = threadData.character,
						.ctx = threadData.ctx,
						.node = node,
						.slot = std::to_underlying(artifact.slot),
						.currentScore = currentScore,
					};

					auto agg = bnb.solve({}, rollsLeft);
					solution.upgradeChance = agg.successfulRolls / static_cast<float>(std::pow(16u, rollsLeft));
					solution.upgradeAverage = (agg.upgradeScores / static_cast<double>(agg.successfulRolls)) / currentScore;

					solutions.addSolution(solution);
				}

				combed++;
				std::println("Max chance: {} {}/{} ({}%)", solutions.maxUpgradeChance, combed.load(), artifactCount, (static_cast<float>(combed) / static_cast<float>(artifactCount)) * 100.f);
			}
		);
	}

	character.state.loadout().artifact.equipped = prevLoadout;
	character.state.loadout().artifact.refreshStats();

	auto end = std::chrono::high_resolution_clock::now();
	std::println("------------------------------------------------");
	std::println("Optimizing done: Best chance {}, time taken {}", solutions.maxUpgradeChance, std::chrono::duration_cast<std::chrono::milliseconds>(end - start));
	for (const auto &[index, solution]: solutions.solutions | std::views::enumerate) {
		if (!solution.artifact) continue;
		std::println("--------------- Artifact {}: Chance {}%, Avg Upgrade {}5 -----------------", index, solution.upgradeChance * 100.f, solution.upgradeAverage * 100.f);
		auto &artifact = ::Store::artifacts.at(solution.artifact);
		std::println("Main stat: {}, Slot: {}, Set: {}", Utils::Stringify(artifact.mainStat), Utils::Stringify(artifact.slot), Artifact::sets.at(artifact.set).name);
		for (const auto &subStat: artifact.subStats) {
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
