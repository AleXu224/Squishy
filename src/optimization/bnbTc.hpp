#pragma once

#include "character/instance.hpp"
#include "solutionTc.hpp"
#include "theorycraftFilter.hpp"
#include <numeric>

namespace Optimization {
	static inline void bnbTc(
		TheorycraftFilter filter,
		SolutionTC &solution,
		Character::Instance &character,
		const RollsValues &rolls,
		const Formula::Context &ctx,
		const Formula::Compiled::FloatNode &node,
		const std::optional<Stat> &splitSlot
	) {
		if (filter.hasTooManyRolls()) return;
		filter.trimImpossibleMaxRolls();

		auto &loadout = character.state.loadout();
		auto maxStats = filter.getMaxStats(rolls);

		// Check if this branch can possibly be good enough to be worth considering
		loadout.artifact.sheet.equippedArtifacts[1] = &maxStats;
		if (node.eval(ctx) <= solution.score) return;

		float maxVariance = 0;
		Stat maxVarianceSlot;

		for (const auto &stat: Stats::subStats) {
			auto varianceFunc = [&](const float &variance) {
				if (variance > maxVariance) {
					maxVariance = variance;
					maxVarianceSlot = stat;
				}
			};
			if (splitSlot.has_value() && splitSlot.value() == stat) {
				varianceFunc(0);
				continue;
			}
			auto &statValues = filter.fromSubStat(stat);
			float maxVal = 0;
			float minVal = std::numeric_limits<float>::max();

			auto &maxStatsStat = maxStats.fromStat(stat);
			auto rollValue = rolls.fromSubStat(stat);
			auto prevMax = maxStatsStat;
			for (auto i = statValues.maxRolls; i > statValues.minRolls; i--) {
				maxStatsStat = static_cast<float>(i) * rollValue;
				auto val = node.eval(ctx);

				if (val < solution.score) {
					statValues.minRolls = i + 1;
					break;
				}

				maxVal = std::max(val, maxVal);
				minVal = std::min(val, minVal);
			}
			maxStatsStat = prevMax;

			auto variance = maxVal - minVal;

			varianceFunc(variance);
		}

		// Bruteforce if the combination count is low enough
		if (filter.getCombCount() <= 32) {
			auto &equipped = loadout.artifact.getTheorycraft();

			for (const auto &stat: Stats::subStats) {
				equipped.fromStat(stat) = filter.fromSubStat(stat).minRolls;
			}

			filter.iterateRolls(
				[&](uint8_t roll, ::Stat stat) {
					equipped.fromStat(stat) = roll;
				},
				[&]() {
					equipped.updateStats();
					loadout.artifact.refreshStats();
					auto dmg = node.eval(ctx);
					if (dmg > solution.score) {
						solution.score = dmg;
						solution.artifacts = equipped;
					}
				}
			);

			// cleanup
			for (const auto &stat: Stats::subStats) {
				equipped.fromStat(stat) = 0;
			}
			equipped.updateStats();
			loadout.artifact.refreshStats();
			return;
		}

		size_t biggestSlot = 0;
		if (maxVariance == 0) {
			for (const auto &stat: Stats::subStats) {
				auto size = filter.fromSubStat(stat).getRange();
				if (size > biggestSlot) {
					biggestSlot = size;
					maxVarianceSlot = stat;
				}
			}
		}

		std::optional<Stat> splitSlotRet = maxVarianceSlot;

		auto &chosenSplitSlot = filter.fromSubStat(maxVarianceSlot);
		auto midPoint = std::midpoint(chosenSplitSlot.minRolls, chosenSplitSlot.maxRolls);
		auto copy1 = filter;
		auto copy2 = filter;

		copy1.fromSubStat(maxVarianceSlot).maxRolls = midPoint;
		copy2.fromSubStat(maxVarianceSlot).minRolls = midPoint + 1;

		bnbTc(copy1, solution, character, rolls, ctx, node, splitSlotRet);
		bnbTc(copy2, solution, character, rolls, ctx, node, splitSlotRet);
	}
}// namespace Optimization