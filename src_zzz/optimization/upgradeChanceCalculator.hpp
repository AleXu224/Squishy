#pragma once

#include "agent/instance.hpp"
#include "disc/instance.hpp"
#include "optimization/bnbUpgrade.hpp"
#include "stats/disc.hpp"

#include <algorithm>
#include <cstdint>
#include <optional>
#include <vector>

namespace Optimization {
	struct UpgradeChanceCalculator {
		const Disc::Instance &disc;
		Agent::Instance &agent;
		const Formula::Context &ctx;
		const Formula::FloatNode &node;
		uint32_t partition;
		float currentScore;

		struct Result {
			double successfulRolls = 0;
			double upgradeScores = 0;
			double denominator = 0;
		};

		std::optional<Result> compute(uint8_t rollsLeft) const {
			std::vector<size_t> unfilledIndices;
			for (size_t i = 0; i < disc.subStats.size(); i++) {
				if (!disc.subStats.at(i).stat.has_value()) unfilledIndices.push_back(i);
			}

			thread_local std::vector<::Stat> pool;
			pool.clear();
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

			auto slotsToFill = std::min(unfilledIndices.size(), static_cast<size_t>(rollsLeft));
			rollsLeft -= static_cast<uint8_t>(slotsToFill);
			auto bnbRollsLeft = rollsLeft;

			if (bnbRollsLeft == 0 && slotsToFill == 0) return std::nullopt;

			size_t presentCount = 0;
			for (const auto &subStat: disc.subStats) {
				if (subStat.stat.has_value()) presentCount++;
			}
			presentCount += slotsToFill;

			uint64_t perm = 1;
			for (size_t k = 0; k < slotsToFill; k++) {
				perm *= static_cast<uint64_t>(pool.size() - k);
			}
			// perm *= std::pow(4ull, bnbRollsLeft);

			Result result;
			thread_local std::vector<::Stat> chosen;
			chosen.clear();
			chosen.reserve(slotsToFill);

			auto enumerate = [&](this auto &&self, size_t depth) {
				if (depth == slotsToFill) {
					Disc::Instance discCopy = disc;
					BnbUpgrade::State state;
					for (size_t k = 0; k < slotsToFill; k++) {
						auto idx = unfilledIndices.at(k);
						discCopy.subStats.at(idx).stat = chosen.at(k);
						discCopy.subStats.at(idx).activated = false;
						state.count.at(idx) = 1;
					}
					discCopy.level = 15;
					discCopy.updateStats();

					BnbUpgrade bnb{
						.disc = discCopy,
						.agent = agent,
						.ctx = ctx,
						.node = node,
						.partition = partition,
						.currentScore = currentScore,
					};
					auto agg = bnb.solve(state, bnbRollsLeft);
					result.successfulRolls += static_cast<double>(agg.successfulRolls);
					result.upgradeScores += agg.upgradeScores;
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

			// Each remaining upgrade roll lands on one of `presentCount` substats.
			result.denominator = static_cast<double>(perm);
			for (size_t k = 0; k < bnbRollsLeft; k++) {
				result.denominator *= static_cast<double>(presentCount);
			}
			return result;
		}
	};
}// namespace Optimization