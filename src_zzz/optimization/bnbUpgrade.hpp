#pragma once

#include "agent/instance.hpp"
#include "disc/instance.hpp"
#include "stats/stat.hpp"

#include <array>
#include <cmath>
#include <cstdint>
#include <unordered_set>
#include <vector>

namespace Optimization {
	struct BnbUpgrade {
		const Disc::Instance &disc;
		Agent::Instance &agent;
		const Formula::Context &ctx;
		const Formula::FloatNode &node;
		uint32_t partition;
		float currentScore;
		std::array<uint8_t, 4> baseRolls{};
		uint8_t rollsLeft = 0;
		const std::unordered_set<::Stat> *substatDependencies = nullptr;
		mutable std::array<bool, 4> invariantSubstats{};

		void computeInvariantSubstats() const {
			for (size_t i = 0; i < 4; i++) {
				auto stat = disc.subStats.at(i).stat;
				invariantSubstats[i] = !stat.has_value() || (substatDependencies != nullptr && !substatDependencies->contains(stat.value()));
			}
		}

		uint8_t presentCount() const {
			uint8_t ret = 0;
			for (const auto &sub: disc.subStats) {
				if (sub.stat.has_value()) ret++;
			}
			return ret;
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

		static constexpr std::array<uint32_t, 7> factorial{1, 1, 2, 6, 24, 120, 720};

		double rollCountProbability(const std::array<uint8_t, 4> &counts) const {
			double orderings = static_cast<double>(factorial[rollsLeft]);
			for (const auto &count: counts) orderings /= factorial[count];
			return orderings / std::pow(static_cast<double>(presentCount()), rollsLeft);
		}

		void collectCounts(size_t substatIndex, uint8_t remainingRolls, std::array<uint8_t, 4> &counts, std::vector<AggregatedCount> &aggregated) const {
			if (substatIndex == 4) {
				if (remainingRolls != 0) return;
				double probability = rollCountProbability(counts);
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
			auto stat = disc.subStats.at(substatIndex).stat;
			if (!stat.has_value()) {
				counts[substatIndex] = 0;
				collectCounts(substatIndex + 1, remainingRolls, counts, aggregated);
				return;
			}
			for (uint8_t count = 0; count <= remainingRolls; count++) {
				counts[substatIndex] = count;
				collectCounts(substatIndex + 1, remainingRolls - count, counts, aggregated);
			}
		}

		void addRollValues(auto &sheet, const std::array<uint8_t, 4> &counts) const {
			for (size_t i = 0; i < 4; i++) {
				auto stat = disc.subStats.at(i).stat;
				if (!stat.has_value() || invariantSubstats.at(i)) continue;
				uint8_t substatRolls = static_cast<uint8_t>(baseRolls.at(i) + counts.at(i));
				sheet.fromStat(stat.value()) += Stats::Values::subStat.at(stat.value()).fiveStar * static_cast<float>(substatRolls);
			}
		}

		struct Agg {
			double chance = 0;
			double upgradeScores = 0;
		};

		[[nodiscard]] float evalSheet(Stats::SheetSimplified<float> &sheet) const {
			auto &loadout = agent.state.loadout();
			loadout.disc.sheet.equippedDiscs.at(partition) = &sheet;
			return node.eval(ctx);
		}

		Agg solve() const {
			computeInvariantSubstats();

			std::array<uint8_t, 4> counts{};
			std::vector<AggregatedCount> aggregated;
			collectCounts(0, rollsLeft, counts, aggregated);

			Agg agg{};
			for (const auto &entry: aggregated) {
				auto sheet = disc.stats;
				addRollValues(sheet, entry.counts);
				auto score = evalSheet(sheet);
				if (score > currentScore) {
					agg.chance += entry.probability;
					agg.upgradeScores += entry.probability * (score - currentScore);
				}
			}
			return agg;
		}
	};
}// namespace Optimization
