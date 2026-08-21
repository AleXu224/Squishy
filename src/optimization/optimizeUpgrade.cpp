#include "optimizeUpgrade.hpp"

#include "chrono"
#include "execution"
#include "formula/substatDependencies.hpp"
#include "optimization/bnbUpgrade.hpp"
#include "optimization/definitionChanceCalculator.hpp"
#include "optimization/optimize.hpp"
#include "optimization/rerollChanceCalculator.hpp"
#include "solutionUpgrade.hpp"
#include "stats/artifact.hpp"
#include "stats/stat.hpp"
#include "store.hpp"
#include "upgradeFilter.hpp"

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <unordered_set>
#include <utility>
#include <vector>


namespace {
	constexpr size_t exactTopN = 20;

	template<typename Calc>
	void runCandidates(
		std::vector<typename Calc::Candidate> &candidates,
		Optimization::SolutionsUpgrade &solutions,
		Character::Instance &character,
		const Formula::Context &ctx,
		const Formula::FloatNode &node,
		const Optimization::OptionsUpgrade &options,
		float currentScore,
		uint64_t runID,
		const std::unordered_set<Stat> &substatDependencies
	) {
		using Candidate = typename Calc::Candidate;

		std::for_each(
			std::execution::parallel_unsequenced_policy{},
			candidates.begin(), candidates.end(),
			[&](Candidate &candidate) {
				thread_local uint64_t localRunID = runID;
				thread_local Optimization::OptimizationThreadData threadData{character, ctx};

				if (localRunID != runID) {
					threadData.~OptimizationThreadData();
					new (&threadData) Optimization::OptimizationThreadData(character, ctx);
					localRunID = runID;
				}

				threadData.character.state.loadout().artifact.refreshStats();

				Calc calc{
					.character = threadData.character,
					.ctx = threadData.ctx,
					.node = node,
					.options = options,
					.currentScore = currentScore,
					.substatDependencies = &substatDependencies,
				};
				candidate.score = calc.cheapScore(candidate);
			}
		);

		std::sort(candidates.begin(), candidates.end(), [](const Candidate &a, const Candidate &b) {
			return a.score > b.score;
		});

		auto exactCount = std::min<size_t>(candidates.size(), exactTopN);
		std::for_each(
			std::execution::parallel_unsequenced_policy{},
			candidates.begin(), candidates.begin() + static_cast<ptrdiff_t>(exactCount),
			[&](Candidate &candidate) {
				thread_local uint64_t localRunID = runID;
				thread_local Optimization::OptimizationThreadData threadData{character, ctx};

				if (localRunID != runID) {
					threadData.~OptimizationThreadData();
					new (&threadData) Optimization::OptimizationThreadData(character, ctx);
					localRunID = runID;
				}

				threadData.character.state.loadout().artifact.refreshStats();

				Calc calc{
					.character = threadData.character,
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
}// namespace


Optimization::SolutionsUpgrade Optimization::UpgradeOptimization::optimize() const {
	auto start = std::chrono::high_resolution_clock::now();
	auto prevLoadout = character.state.loadout().artifact.getSlotted();

	auto mainStatEnabled = [&](const Artifact::Instance &artifact) {
		switch (artifact.slot) {
			case Artifact::Slot::sands:
				return options.sandsMainStats.at(artifact.mainStat);
			case Artifact::Slot::goblet:
				return options.gobletMainStats.at(artifact.mainStat);
			case Artifact::Slot::circlet:
				return options.circletMainStats.at(artifact.mainStat);
			default:
				return true;
		}
	};

	std::vector<Artifact::Instance> artifacts;
	artifacts.reserve(::Store::artifacts.size());
	for (const auto &[_, artifact]: ::Store::artifacts) {
		if (artifact.level >= 20) continue;
		if (!mainStatEnabled(artifact)) continue;
		artifacts.emplace_back(artifact);
	}

	std::vector<Artifact::Instance> rerollArtifacts;
	if (options.enableReroll) {
		rerollArtifacts.reserve(::Store::artifacts.size());
		for (const auto &[_, artifact]: ::Store::artifacts) {
			if (artifact.level < 20) continue;
			if (!mainStatEnabled(artifact)) continue;
			rerollArtifacts.emplace_back(artifact);
		}
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
			auto filtered = filter.filter(artifacts);
			std::for_each(
				std::execution::parallel_unsequenced_policy{},
				filtered.begin(), filtered.end(),
				[&combed, &solutions, &node, &substatDependencies, &character_original = character, artifactCount = artifacts.size(), &initialCtx = ctx, runID = runID, currentScore](Artifact::Instance &artifact) {
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
							.rollsLeft = static_cast<uint8_t>(rollsLeft),
							.substatDependencies = &substatDependencies,
						};

						auto agg = bnb.solve(true);
						SolutionUpgrade solution{
							.upgradeChance = static_cast<float>(agg.chance),
							.upgradeAverage = static_cast<float>(agg.chance > 0 ? (agg.upgradeScores / agg.chance / currentScore) : 0),
							.score = 0.f,
							.artifact = artifact.key,
							.kind = SolutionUpgrade::Kind::levelUp,
						};
						solution.score = solution.upgradeChance * solution.upgradeAverage;
						solutions.addSolution(solution);
					}

					combed++;
					std::println("Max score: {} {}/{} ({}%)", solutions.maxScore, combed.load(), artifactCount, (static_cast<float>(combed) / static_cast<float>(artifactCount)) * 100.f);
				}
			);
		}
	}

	if (options.enableReroll) {
		std::vector<RerollChanceCalculator::Candidate> rerollCandidates;
		for (auto &filter: filters) {
			auto filtered = filter.filter(rerollArtifacts);
			for (auto &artifact: filtered) {
				auto candidates = RerollChanceCalculator::makeCandidates(artifact, options);
				rerollCandidates.insert(rerollCandidates.end(), candidates.begin(), candidates.end());
			}
		}
		if (!rerollCandidates.empty()) {
			runCandidates<RerollChanceCalculator>(rerollCandidates, solutions, character, ctx, node, options, currentScore, runID, substatDependencies);
		}
	}

	if (options.enableDefinition) {
		std::vector<DefinitionChanceCalculator::Candidate> defCandidates;
		for (const auto &[index, slot]: Artifact::slots | std::views::enumerate) {
			if (!filters.at(index).set) continue;
			auto candidates = DefinitionChanceCalculator::makeCandidates(filters.at(index).set, slot, options);
			defCandidates.insert(defCandidates.end(), candidates.begin(), candidates.end());
		}
		if (!defCandidates.empty()) {
			runCandidates<DefinitionChanceCalculator>(defCandidates, solutions, character, ctx, node, options, currentScore, runID, substatDependencies);
		}
	}

	character.state.loadout().artifact.equipped = prevLoadout;
	character.state.loadout().artifact.refreshStats();

	auto end = std::chrono::high_resolution_clock::now();
	std::println("------------------------------------------------");
	std::println("Optimizing done: Best score {}, time taken {}", solutions.maxScore, std::chrono::duration_cast<std::chrono::milliseconds>(end - start));
	for (const auto &[index, solution]: solutions.solutions | std::views::enumerate) {
		if (solution.kind == SolutionUpgrade::Kind::definition) {
			std::println("--------------- Definition {}: Chance {}%, Avg Upgrade {}% -----------------", index, solution.upgradeChance * 100.f, solution.upgradeAverage * 100.f);
			std::println("Set: {}, Slot: {}, Main stat: {}", Artifact::sets.at(solution.set).name, Utils::Stringify(solution.slot), Utils::Stringify(solution.mainStat));
			std::println("Chosen sub stats: {} + {} ({} rolls)", Utils::Stringify(solution.chosenSubStats.at(0)), Utils::Stringify(solution.chosenSubStats.at(1)), solution.guaranteedRolls);
			continue;
		}
		if (!solution.artifact) continue;
		std::println("--------------- Artifact {}: Chance {}%, Avg Upgrade {}% -----------------", index, solution.upgradeChance * 100.f, solution.upgradeAverage * 100.f);
		auto &artifact = ::Store::artifacts.at(solution.artifact);
		std::println("Main stat: {}, Slot: {}, Set: {}", Utils::Stringify(artifact.mainStat), Utils::Stringify(artifact.slot), Artifact::sets.at(artifact.set).name);
		if (solution.kind == SolutionUpgrade::Kind::reroll) {
			std::println("Reroll: {} + {} ({} rolls)", Utils::Stringify(solution.chosenSubStats.at(0)), Utils::Stringify(solution.chosenSubStats.at(1)), solution.guaranteedRolls);
		}
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
