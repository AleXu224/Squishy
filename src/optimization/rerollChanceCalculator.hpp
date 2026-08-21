#pragma once

#include "artifact/instance.hpp"
#include "character/instance.hpp"
#include "optimization/bnbUpgrade.hpp"
#include "optimization/optionsUpgrade.hpp"
#include "optimization/solutionUpgrade.hpp"
#include "stats/stat.hpp"

#include <array>
#include <cstdint>
#include <unordered_set>
#include <utility>
#include <vector>

namespace Optimization {
	struct RerollChanceCalculator {
		Character::Instance &character;
		const Formula::Context &ctx;
		const Formula::FloatNode &node;
		const OptionsUpgrade &options;
		float currentScore;
		const std::unordered_set<::Stat> *substatDependencies = nullptr;

		struct Candidate {
			Artifact::InstanceKey key;
			Artifact::Instance baseline;
			std::array<uint8_t, 2> guaranteedSubstats;
			std::array<Stat, 2> chosenStats;
			uint8_t rollsLeft;
			double score;
		};

		static std::vector<Candidate> makeCandidates(const Artifact::Instance &artifact, const OptionsUpgrade &options) {
			std::vector<Candidate> ret;
			if (artifact.rarity != 5 || artifact.level != 20) return ret;
			if (!artifact.totalRolls.has_value()) return ret;
			for (const auto &sub: artifact.subStats) {
				if (!sub.stat.has_value() || !sub.initialValue.has_value()) return ret;
			}

			Artifact::Instance baseline = artifact;
			for (auto &sub: baseline.subStats) {
				sub.value = sub.initialValue.value();
			}
			baseline.updateStats();
			auto rollsLeft = static_cast<uint8_t>(artifact.totalRolls.value() - 4);

			for (uint8_t i = 0; i < 4; i++) {
				auto statI = baseline.subStats.at(i).stat;
				if (!statI.has_value()) continue;
				if (!options.guaranteedSubStats.at(statI.value())) continue;
				for (uint8_t j = static_cast<uint8_t>(i + 1); j < 4; j++) {
					auto statJ = baseline.subStats.at(j).stat;
					if (!statJ.has_value()) continue;
					if (!options.guaranteedSubStats.at(statJ.value())) continue;
					ret.push_back(Candidate{
						.key = artifact.key,
						.baseline = baseline,
						.guaranteedSubstats = {i, j},
						.chosenStats = {statI.value(), statJ.value()},
						.rollsLeft = rollsLeft,
						.score = 0,
					});
				}
			}
			return ret;
		}

		BnbUpgrade makeBnb(const Candidate &candidate) const {
			return BnbUpgrade{
				.artifact = candidate.baseline,
				.character = character,
				.ctx = ctx,
				.node = node,
				.slot = std::to_underlying(candidate.baseline.slot),
				.currentScore = currentScore,
				.guaranteedSubstats = candidate.guaranteedSubstats,
				.guaranteedRolls = options.rerollGuaranteedRolls,
				.baseRolls = {0, 0, 0, 0},
				.rollsLeft = candidate.rollsLeft,
				.substatDependencies = substatDependencies,
			};
		}

		double cheapScore(const Candidate &candidate) const {
			auto bnb = makeBnb(candidate);
			auto agg = bnb.solve(false);
			if (agg.chance <= 0) return 0;
			return (agg.upgradeScores / agg.chance / currentScore) * agg.chance;
		}

		SolutionUpgrade exactSolution(const Candidate &candidate) const {
			auto bnb = makeBnb(candidate);
			auto agg = bnb.solve(true);
			SolutionUpgrade solution{
				.upgradeChance = static_cast<float>(agg.chance),
				.upgradeAverage = static_cast<float>(agg.chance > 0 ? (agg.upgradeScores / agg.chance / currentScore) : 0),
				.score = 0.f,
				.artifact = candidate.key,
				.kind = SolutionUpgrade::Kind::reroll,
				.set = candidate.baseline.set,
				.slot = candidate.baseline.slot,
				.mainStat = candidate.baseline.mainStat,
				.chosenSubStats = candidate.chosenStats,
				.guaranteedRolls = options.rerollGuaranteedRolls,
			};
			solution.score = solution.upgradeChance * solution.upgradeAverage;
			return solution;
		}
	};
}// namespace Optimization
