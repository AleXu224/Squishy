#pragma once

#include "agent/instance.hpp"
#include "disc/instance.hpp"
#include "disc/slot.hpp"
#include "optimization/bnbUpgrade.hpp"
#include "optimization/optionsUpgrade.hpp"
#include "optimization/solutionUpgrade.hpp"
#include "stats/disc.hpp"
#include "stats/stat.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <unordered_set>
#include <utility>
#include <vector>

namespace Optimization {
	struct DefinitionChanceCalculator {
		Agent::Instance &agent;
		const Formula::Context &ctx;
		const Formula::FloatNode &node;
		const OptionsUpgrade &options;
		float currentScore;
		const std::unordered_set<::Stat> *substatDependencies = nullptr;

		static constexpr double prob4sub = 0.5;

		struct Candidate {
			Disc::SetKey set;
			Disc::Partition partition;
			Stat mainStat;
			std::array<Stat, 2> chosen{};
			uint8_t chosenCount = 0;
			double score;
		};

		struct Agg {
			double chance;
			double upgradeScores;
		};

		static std::vector<Candidate> makeCandidates(Disc::SetKey set, Disc::Partition partition, const OptionsUpgrade &options) {
			std::vector<Candidate> ret;
			for (const auto &mainStat: Stats::Disc::byPartition(partition)) {
				switch (partition) {
					case Disc::Partition::four:
						if (!options.partition4MainStats.at(mainStat)) continue;
						break;
					case Disc::Partition::five:
						if (!options.partition5MainStats.at(mainStat)) continue;
						break;
					case Disc::Partition::six:
						if (!options.partition6MainStats.at(mainStat)) continue;
						break;
					default:
						break;
				}

				std::vector<Stat> pool;
				for (const auto &stat: Stats::Disc::subStats) {
					if (stat == mainStat) continue;
					pool.emplace_back(stat);
				}

				ret.push_back(Candidate{
					.set = set,
					.partition = partition,
					.mainStat = mainStat,
					.chosen = {},
					.chosenCount = 0,
					.score = 0,
				});
				for (const auto &stat: pool) {
					if (!options.guaranteedSubStats.at(stat)) continue;
					ret.push_back(Candidate{
						.set = set,
						.partition = partition,
						.mainStat = mainStat,
						.chosen = {stat},
						.chosenCount = 1,
						.score = 0,
					});
				}
				for (size_t pi = 0; pi < pool.size(); pi++) {
					auto chosen1 = pool.at(pi);
					if (!options.guaranteedSubStats.at(chosen1)) continue;
					for (size_t pj = pi + 1; pj < pool.size(); pj++) {
						auto chosen2 = pool.at(pj);
						if (!options.guaranteedSubStats.at(chosen2)) continue;
						ret.push_back(Candidate{
							.set = set,
							.partition = partition,
							.mainStat = mainStat,
							.chosen = {chosen1, chosen2},
							.chosenCount = 2,
							.score = 0,
						});
					}
				}
			}
			return ret;
		}

		Agg run(const Candidate &candidate) const {
			std::vector<Stat> pool;
			for (const auto &stat: Stats::Disc::subStats) {
				if (stat == candidate.mainStat) continue;
				bool isChosen = false;
				for (uint8_t i = 0; i < candidate.chosenCount; i++) {
					if (stat == candidate.chosen.at(i)) isChosen = true;
				}
				if (isChosen) continue;
				pool.emplace_back(stat);
			}

			auto extraCount = static_cast<uint8_t>(4 - candidate.chosenCount);

			struct Group {
				std::vector<Stat> dependent;
				std::array<Stat, 4> extras{};
				uint8_t extraCount = 0;
				double probability = 0;
			};
			std::vector<Group> groups;

			auto combinationCount = [&](uint8_t total, uint8_t picked) {
				double ret = 1;
				for (uint8_t k = 1; k <= picked; k++) {
					ret = ret * static_cast<double>(total - k + 1) / static_cast<double>(k);
				}
				return ret;
			};
			auto combinationProbability = 1.0 / combinationCount(static_cast<uint8_t>(pool.size()), extraCount);

			std::vector<Stat> combo;
			combo.reserve(extraCount);
			auto enumerate = [&](this auto &&self, size_t start, uint8_t remaining) {
				if (remaining == 0) {
					std::vector<Stat> dependent;
					for (const auto &stat: combo) {
						if (substatDependencies && substatDependencies->contains(stat)) {
							dependent.emplace_back(stat);
						}
					}
					for (auto &group: groups) {
						if (group.dependent == dependent) {
							group.probability += combinationProbability;
							return;
						}
					}
					auto extras = std::array<Stat, 4>{};
					std::copy(combo.begin(), combo.end(), extras.begin());
					groups.push_back({.dependent = std::move(dependent), .extras = extras, .extraCount = extraCount, .probability = combinationProbability});
					return;
				}
				for (size_t i = start; i + remaining <= pool.size(); i++) {
					combo.push_back(pool.at(i));
					self(i + 1, static_cast<uint8_t>(remaining - 1));
					combo.pop_back();
				}
			};
			enumerate(0, extraCount);

			Agg agg{0, 0};
			for (const auto &group: groups) {
				Disc::Instance baseline{};
				baseline.set = candidate.set;
				baseline.partition = candidate.partition;
				baseline.mainStat = candidate.mainStat;
				baseline.level = 15;
				for (uint8_t i = 0; i < candidate.chosenCount; i++) {
					baseline.subStats.at(i) = DiscSubstat{.stat = candidate.chosen.at(i), .activated = false, .rolls = 0};
				}
				for (uint8_t i = 0; i < group.extraCount; i++) {
					baseline.subStats.at(candidate.chosenCount + i) = DiscSubstat{.stat = group.extras.at(i), .activated = false, .rolls = 0};
				}
				baseline.updateStats();

				BnbUpgrade bnb{
					.disc = baseline,
					.agent = agent,
					.ctx = ctx,
					.node = node,
					.partition = std::to_underlying(candidate.partition),
					.currentScore = currentScore,
					.baseRolls = {1, 1, 1, 1},
					.rollsLeft = 5,
					.substatDependencies = substatDependencies,
				};
				auto agg4 = bnb.solve();

				auto bnb3 = bnb;
				bnb3.rollsLeft = 4;
				auto agg3 = bnb3.solve();

				auto chance = prob4sub * agg4.chance + (1.0 - prob4sub) * agg3.chance;
				auto upgradeScores = prob4sub * agg4.upgradeScores + (1.0 - prob4sub) * agg3.upgradeScores;
				agg.chance += group.probability * chance;
				agg.upgradeScores += group.probability * upgradeScores;
			}
			return agg;
		}

		SolutionUpgrade exactSolution(const Candidate &candidate) const {
			auto agg = run(candidate);
			SolutionUpgrade solution{
				.upgradeChance = static_cast<float>(agg.chance),
				.upgradeAverage = static_cast<float>(agg.chance > 0 ? (agg.upgradeScores / agg.chance / currentScore) : 0),
				.score = 0.f,
				.disc = {},
				.kind = SolutionUpgrade::Kind::definition,
				.set = candidate.set,
				.partition = candidate.partition,
				.mainStat = candidate.mainStat,
				.chosenSubStats = candidate.chosen,
				.chosenCount = candidate.chosenCount,
			};
			solution.score = solution.upgradeChance * solution.upgradeAverage;
			return solution;
		}
	};
}// namespace Optimization
