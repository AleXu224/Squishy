#pragma once

#include "artifact/instance.hpp"
#include "character/instance.hpp"
#include "stats/stat.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <unordered_set>
#include <utility>
#include <vector>

namespace Optimization {
	struct BnbUpgrade {
		const Artifact::Instance &artifact;
		Character::Instance &character;
		const Formula::Context &ctx;
		const Formula::FloatNode &node;
		uint32_t slot;
		float currentScore;
		std::array<uint8_t, 2> guaranteedSubstats{};
		uint8_t guaranteedRolls = 0;
		std::array<uint8_t, 4> baseRolls{};
		uint8_t rollsLeft = 0;
		const std::unordered_set<::Stat> *substatDependencies = nullptr;
		mutable std::array<bool, 4> invariantSubstats{};

		void computeInvariantSubstats() const {
			for (size_t i = 0; i < 4; i++) {
				auto stat = artifact.subStats.at(i).stat;
				invariantSubstats[i] = !stat.has_value() || (substatDependencies != nullptr && !substatDependencies->contains(stat.value()));
			}
		}

		uint32_t reducedKey(const std::array<uint8_t, 4> &counts) const {
			uint32_t key = 0;
			uint32_t shift = 0;
			for (size_t i = 0; i < 4; i++) {
				if (invariantSubstats.at(i)) continue;
				key |= static_cast<uint32_t>(counts.at(i)) << shift;
				shift += 8;
			}
			return key;
		}

		struct AggregatedCount {
			std::array<uint8_t, 4> counts{};
			uint32_t key = 0;
			double probability = 0;
		};

		void collectCounts(size_t substatIndex, uint8_t remainingRolls, std::array<uint8_t, 4> &counts, std::vector<AggregatedCount> &aggregated, double chosenWithinMin) const {
			if (substatIndex == 3) {
				counts[3] = remainingRolls;
				double probability = rollCountProbability(counts, chosenWithinMin);
				if (probability == 0) return;
				auto key = reducedKey(counts);
				for (auto &entry: aggregated) {
					if (entry.key == key) {
						entry.probability += probability;
						return;
					}
				}
				aggregated.push_back({.counts = counts, .key = key, .probability = probability});
				return;
			}
			for (uint8_t count = 0; count <= remainingRolls; count++) {
				counts[substatIndex] = count;
				collectCounts(substatIndex + 1, remainingRolls - count, counts, aggregated, chosenWithinMin);
			}
		}

		static constexpr std::array<uint32_t, 7> factorial{1, 1, 2, 6, 24, 120, 720};

		static constexpr uint32_t waysToPick(uint8_t total, uint8_t picked) {
			return factorial[total] / (factorial[picked] * factorial[total - picked]);
		}

		static constexpr uint32_t waysToReachSum(uint8_t rolls, uint8_t targetSum) {
			static constexpr std::array<std::array<uint32_t, 19>, 7> waysTable = [] {
				std::array<std::array<uint32_t, 19>, 7> table{};
				for (size_t rollCount = 0; rollCount <= 6; rollCount++) {
					for (size_t target = 0; target <= 3 * rollCount; target++) {
						if (rollCount == 0) {
							table[rollCount][target] = target == 0 ? 1 : 0;
						} else {
							uint32_t ways = 0;
							for (size_t lastRoll = 0; lastRoll <= 3 && lastRoll <= target; lastRoll++) {
								ways += table[rollCount - 1][target - lastRoll];
							}
							table[rollCount][target] = ways;
						}
					}
				}
				return table;
			}();
			return waysTable[rolls][targetSum];
		}

		struct Agg {
			double chance = 0;
			double upgradeScores = 0;
		};

		struct CountOption {
			std::array<uint8_t, 4> counts;
			double probability;
			Stats::SheetSimplified<float> maxSheet;
			float maxScore;
		};

		static double chosenWithinMinChance(uint8_t rollsLeft, uint8_t guaranteedRolls) {
			double chance = 0;
			for (uint8_t chosenCount = 0; chosenCount <= guaranteedRolls; chosenCount++) {
				chance += static_cast<double>(waysToPick(rollsLeft, chosenCount));
			}
			return chance / static_cast<double>(1u << rollsLeft);
		}

		double rollCountProbability(const std::array<uint8_t, 4> &counts, double chosenWithinMin) const {
			uint8_t rollsIntoChosen = counts[guaranteedSubstats[0]] + counts[guaranteedSubstats[1]];

			double orderings = static_cast<double>(factorial[rollsLeft]);
			for (const auto &count: counts) orderings /= factorial[count];
			double naturalProbability = orderings / static_cast<double>(1u << (2 * rollsLeft));

			if (guaranteedRolls == 0 || rollsIntoChosen > guaranteedRolls) return naturalProbability;
			if (rollsIntoChosen < guaranteedRolls) return 0;

			double splitProbability = static_cast<double>(factorial[guaranteedRolls]) * factorial[rollsLeft - guaranteedRolls];
			for (const auto &count: counts) splitProbability /= factorial[count];
			splitProbability /= static_cast<double>(1u << rollsLeft);

			return chosenWithinMin * splitProbability;
		}

		void addRollValues(auto &sheet, const std::array<uint8_t, 4> &counts, auto valuePerRoll) const {
			for (size_t i = 0; i < 4; i++) {
				auto stat = artifact.subStats.at(i).stat;
				if (!stat.has_value() || invariantSubstats.at(i)) continue;
				uint8_t substatRolls = static_cast<uint8_t>(baseRolls.at(i) + counts.at(i));
				sheet.fromStat(stat.value()) += static_cast<float>(substatRolls) * valuePerRoll(stat.value());
			}
		}

		void enumerateCheap(Agg &agg) const {
			std::array<uint8_t, 4> counts{};
			std::vector<AggregatedCount> aggregated;
			collectCounts(0, rollsLeft, counts, aggregated, chosenWithinMinChance(rollsLeft, guaranteedRolls));
			for (const auto &entry: aggregated) {
				auto sheet = artifact.stats;
				addRollValues(sheet, entry.counts, [](Stat stat) { return Stats::Values::averageSubStat.at(stat).at(4); });
				auto &loadout = character.state.loadout();
				loadout.artifact.sheet.equippedArtifacts.at(slot) = &sheet;
				auto score = node.eval(ctx);
				if (score > currentScore) {
					agg.chance += entry.probability;
					agg.upgradeScores += entry.probability * (score - currentScore);
				}
			}
		}

		void collectCountOptions(std::vector<CountOption> &options) const {
			std::array<uint8_t, 4> counts{};
			std::vector<AggregatedCount> aggregated;
			collectCounts(0, rollsLeft, counts, aggregated, chosenWithinMinChance(rollsLeft, guaranteedRolls));
			for (const auto &entry: aggregated) {
				CountOption option{
					.counts = entry.counts,
					.probability = entry.probability,
				};
				option.maxSheet = artifact.stats;
				addRollValues(option.maxSheet, entry.counts, [](Stat stat) { return Stats::Values::subStat.at(stat).star5.at(3); });
				options.push_back(std::move(option));
			}
		}

		[[nodiscard]] bool recurseSumsExact(size_t substatIndex, const std::array<uint8_t, 4> &counts, Stats::SheetSimplified<float> &sheet, double probability, Agg &agg) const {
			if (substatIndex == 4) {
				auto &loadout = character.state.loadout();
				loadout.artifact.sheet.equippedArtifacts.at(slot) = &sheet;
				auto score = node.eval(ctx);
				if (score > currentScore) {
					agg.chance += probability;
					agg.upgradeScores += probability * (score - currentScore);
				} else {
					return true;
				}
				return false;
			}
			auto stat = artifact.subStats.at(substatIndex).stat;
			if (!stat.has_value() || invariantSubstats.at(substatIndex)) {
				return recurseSumsExact(substatIndex + 1, counts, sheet, probability, agg);
			}

			uint8_t substatRolls = static_cast<uint8_t>(baseRolls.at(substatIndex) + counts.at(substatIndex));
			const auto &tiers = Stats::Values::subStat.at(stat.value()).star5;
			float minTier = tiers.at(0);
			float tierStep = (tiers.at(3) - tiers.at(0)) / 3.f;

			for (int32_t extraSteps = 3 * substatRolls; extraSteps >= 0; extraSteps--) {
				auto sumProbability = static_cast<double>(waysToReachSum(substatRolls, static_cast<uint8_t>(extraSteps))) / static_cast<double>(1u << (2 * substatRolls));
				if (sumProbability == 0) continue;
				float value = static_cast<float>(substatRolls) * minTier + static_cast<float>(extraSteps) * tierStep;

				sheet.fromStat(stat.value()) += value;
				auto downgrade = recurseSumsExact(substatIndex + 1, counts, sheet, probability * sumProbability, agg);
				sheet.fromStat(stat.value()) -= value;
				if (downgrade) return extraSteps == 3 * substatRolls;
			}
			return false;
		}

		Agg solveExact() const {
			std::vector<CountOption> options;
			collectCountOptions(options);

			auto &loadout = character.state.loadout();
			for (auto &option: options) {
				loadout.artifact.sheet.equippedArtifacts.at(slot) = &option.maxSheet;
				option.maxScore = node.eval(ctx);
			}
			std::sort(options.begin(), options.end(), [](const CountOption &a, const CountOption &b) {
				return a.maxScore > b.maxScore;
			});

			Agg agg{};
			auto sheet = artifact.stats;
			for (const auto &option: options) {
				if (option.maxScore <= currentScore) break;
				sheet = artifact.stats;
				(void) recurseSumsExact(0, option.counts, sheet, option.probability, agg);
			}
			return agg;
		}

		Agg solve(bool exact) const {
			computeInvariantSubstats();
			Agg agg{};
			if (exact) {
				agg = solveExact();
			} else {
				enumerateCheap(agg);
			}
			return agg;
		}
	};
}// namespace Optimization
