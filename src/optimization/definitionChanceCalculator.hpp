#pragma once

#include "artifact/instance.hpp"
#include "artifact/slot.hpp"
#include "character/instance.hpp"
#include "optimization/bnbUpgrade.hpp"
#include "optimization/optionsUpgrade.hpp"
#include "optimization/solutionUpgrade.hpp"
#include "stats/artifact.hpp"
#include "stats/stat.hpp"

#include <array>
#include <cstdint>
#include <unordered_set>
#include <utility>
#include <vector>

namespace Optimization {
	struct DefinitionChanceCalculator {
		Character::Instance &character;
		const Formula::Context &ctx;
		const Formula::FloatNode &node;
		const OptionsUpgrade &options;
		float currentScore;
		const std::unordered_set<::Stat> *substatDependencies = nullptr;

		static constexpr double prob4line = 0.34;

		struct Candidate {
			Artifact::SetKey set;
			Artifact::Slot slot;
			Stat mainStat;
			std::array<Stat, 2> chosen;
			double score;
		};

		struct Agg {
			double chance;
			double upgradeScores;
		};

		static uint32_t substatWeight(Stat stat) {
			switch (stat) {
				case Stat::hp:
				case Stat::atk:
				case Stat::def:
					return 6;
				case Stat::cr:
				case Stat::cd:
					return 3;
				default:
					return 4;
			}
		}

		static std::vector<Candidate> makeCandidates(Artifact::SetKey set, Artifact::Slot slot, const OptionsUpgrade &options) {
			std::vector<Candidate> ret;
			for (const auto &mainStat: Stats::Artifact::bySlot(slot)) {
				switch (slot) {
					case Artifact::Slot::sands:
						if (!options.sandsMainStats.at(mainStat)) continue;
						break;
					case Artifact::Slot::goblet:
						if (!options.gobletMainStats.at(mainStat)) continue;
						break;
					case Artifact::Slot::circlet:
						if (!options.circletMainStats.at(mainStat)) continue;
						break;
					default:
						break;
				}

				std::vector<Stat> pool;
				for (const auto &stat: Stats::subStats) {
					if (stat == mainStat) continue;
					pool.emplace_back(stat);
				}

				for (size_t pi = 0; pi < pool.size(); pi++) {
					auto chosen1 = pool.at(pi);
					if (!options.guaranteedSubStats.at(chosen1)) continue;
					for (size_t pj = pi + 1; pj < pool.size(); pj++) {
						auto chosen2 = pool.at(pj);
						if (!options.guaranteedSubStats.at(chosen2)) continue;
						ret.push_back(Candidate{
							.set = set,
							.slot = slot,
							.mainStat = mainStat,
							.chosen = {chosen1, chosen2},
							.score = 0,
						});
					}
				}
			}
			return ret;
		}

		Agg run(const Candidate &candidate, bool exact) const {
			std::vector<Stat> pool;
			std::vector<Stat> dependent;
			std::vector<Stat> invariant;
			for (const auto &stat: Stats::subStats) {
				if (stat == candidate.mainStat) continue;
				if (stat == candidate.chosen[0] || stat == candidate.chosen[1]) continue;
				pool.emplace_back(stat);
				(substatDependencies && substatDependencies->contains(stat) ? dependent : invariant).emplace_back(stat);
			}

			double totalWeight = 0;
			for (const auto &stat: pool) totalWeight += substatWeight(stat);

			auto pairProbability = [&](Stat a, Stat b) {
				auto weightA = static_cast<double>(substatWeight(a));
				auto weightB = static_cast<double>(substatWeight(b));
				return (weightA / totalWeight) * (weightB / (totalWeight - weightA)) + (weightB / totalWeight) * (weightA / (totalWeight - weightB));
			};

			struct Group {
				std::array<Stat, 2> extras;
				double probability;
			};
			std::vector<Group> groups;

			if (invariant.size() >= 2) {
				double probability = 0;
				for (size_t i = 0; i < invariant.size(); i++)
					for (size_t j = i + 1; j < invariant.size(); j++)
						probability += pairProbability(invariant.at(i), invariant.at(j));
				groups.push_back({{invariant.at(0), invariant.at(1)}, probability});
			}

			if (!invariant.empty()) {
				for (const auto &stat: dependent) {
					double probability = 0;
					for (const auto &other: invariant) probability += pairProbability(stat, other);
					groups.push_back({{stat, invariant.at(0)}, probability});
				}
			}

			for (size_t i = 0; i < dependent.size(); i++) {
				for (size_t j = i + 1; j < dependent.size(); j++) {
					groups.push_back({{dependent.at(i), dependent.at(j)}, pairProbability(dependent.at(i), dependent.at(j))});
				}
			}

			Agg agg{0, 0};
			for (const auto &group: groups) {
				Artifact::Instance baseline{};
				baseline.set = candidate.set;
				baseline.slot = candidate.slot;
				baseline.mainStat = candidate.mainStat;
				baseline.level = 20;
				baseline.subStats.at(0) = StatValue{.stat = candidate.chosen[0], .value = 0.f};
				baseline.subStats.at(1) = StatValue{.stat = candidate.chosen[1], .value = 0.f};
				baseline.subStats.at(2) = StatValue{.stat = group.extras.at(0), .value = 0.f};
				baseline.subStats.at(3) = StatValue{.stat = group.extras.at(1), .value = 0.f};
				baseline.updateStats();

				BnbUpgrade bnb4{
					.artifact = baseline,
					.character = character,
					.ctx = ctx,
					.node = node,
					.slot = std::to_underlying(candidate.slot),
					.currentScore = currentScore,
					.guaranteedSubstats = {0, 1},
					.guaranteedRolls = 2,
					.baseRolls = {1, 1, 1, 1},
					.rollsLeft = 5,
					.substatDependencies = substatDependencies,
				};
				auto agg4 = bnb4.solve(exact);

				auto bnb3 = bnb4;
				bnb3.rollsLeft = 4;
				auto agg3 = bnb3.solve(exact);

				auto chance = prob4line * agg4.chance + (1.0 - prob4line) * agg3.chance;
				auto upgradeScores = prob4line * agg4.upgradeScores + (1.0 - prob4line) * agg3.upgradeScores;
				agg.chance += group.probability * chance;
				agg.upgradeScores += group.probability * upgradeScores;
			}
			return agg;
		}

		double cheapScore(const Candidate &candidate) const {
			auto agg = run(candidate, false);
			if (agg.chance <= 0) return 0;
			return (agg.upgradeScores / agg.chance / currentScore) * agg.chance;
		}

		SolutionUpgrade exactSolution(const Candidate &candidate) const {
			auto agg = run(candidate, true);
			SolutionUpgrade solution{
				.upgradeChance = static_cast<float>(agg.chance),
				.upgradeAverage = static_cast<float>(agg.chance > 0 ? (agg.upgradeScores / agg.chance / currentScore) : 0),
				.score = 0.f,
				.artifact = {},
				.kind = SolutionUpgrade::Kind::definition,
				.set = candidate.set,
				.slot = candidate.slot,
				.mainStat = candidate.mainStat,
				.chosenSubStats = candidate.chosen,
				.guaranteedRolls = 2,
			};
			solution.score = solution.upgradeChance * solution.upgradeAverage;
			return solution;
		}
	};
}// namespace Optimization
