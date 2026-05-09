#pragma once

#include "agent/instance.hpp"
#include "optimization/discFilter.hpp"
#include "solution.hpp"
#include <numeric>

namespace Optimization {
	static inline void bnb(
		FilteredDiscs &discs,
		Solutions &solutions,
		Agent::Instance &agent,
		const Formula::Context &ctx,
		const Formula::FloatNode &node,
		BnbState &state,
		const std::optional<uint32_t> &splitSlot
	) {
		auto &loadout = agent.state.loadout();
		std::array<Stats::Sheet<float>, 6> statsForSlot = getMaxStatsForSlots(discs);

		// Check if this branch can possibly be good enough to be worth considering
		if (!state.isValid()) {
			return;
		}
		state.applySets(loadout);
		loadout.disc.sheet.equippedDiscs[0] = &statsForSlot[0];
		loadout.disc.sheet.equippedDiscs[1] = &statsForSlot[1];
		loadout.disc.sheet.equippedDiscs[2] = &statsForSlot[2];
		loadout.disc.sheet.equippedDiscs[3] = &statsForSlot[3];
		loadout.disc.sheet.equippedDiscs[4] = &statsForSlot[4];
		loadout.disc.sheet.equippedDiscs[5] = &statsForSlot[5];
		if (node.eval(ctx) <= solutions.minScore) return;

		float maxVariance = 0;
		size_t maxVarianceSlot = 0;

		// Check the potential of each disc
		for (size_t i = 0; i < 6; i++) {
			auto varianceFunc = [&](const float &variance) {
				if (variance > maxVariance) {
					maxVariance = variance;
					maxVarianceSlot = i;
				}
			};
			if (splitSlot.has_value() && splitSlot.value() == i) {
				varianceFunc(0);
				continue;
			}
			auto &artis = discs.entries.at(i);
			float maxVal = 0;
			float minVal = std::numeric_limits<float>::max();

			artis.erase(
				std::remove_if(artis.begin(), artis.end(), [&](Disc::Instance *disc) {
					loadout.disc.sheet.equippedDiscs.at(i) = &disc->stats;

					auto val = node.eval(ctx);
					if (val > solutions.minScore) {
						maxVal = std::max(val, maxVal);
						minVal = std::min(val, minVal);
					} else {
						state.subtractCountForIndex(i, disc->set);
					}

					return val <= solutions.minScore;
				}),
				artis.end()
			);
			loadout.disc.sheet.equippedDiscs.at(i) = &statsForSlot.at(i);

			auto variance = maxVal - minVal;
			varianceFunc(variance);
		}

		// Bruteforce if the combination count is low enough
		if (discs.getCombCount() <= 32) {
			auto &equipped = std::get<Stats::Disc::Slotted>(loadout.disc.equipped);
			for (const auto &[one, two, three, four, five, six]: std::views::cartesian_product(discs.entries.at(0), discs.entries.at(1), discs.entries.at(2), discs.entries.at(3), discs.entries.at(4), discs.entries.at(5))) {
				equipped.one = one->key;
				equipped.two = two->key;
				equipped.three = three->key;
				equipped.four = four->key;
				equipped.five = five->key;
				equipped.six = six->key;
				loadout.disc.refreshStats();
				SlotHash hash(loadout.disc.bonus1, loadout.disc.bonus2, loadout.disc.bonus3);
				if (hash != state.targetHash) continue;
				auto dmg = node.eval(ctx);
				solutions.addSolution(equipped, dmg);
			}
			return;
		}

		size_t biggestSlot = 0;
		// size_t biggestSlotIndex = 0;
		if (maxVariance == 0) {
			for (size_t i = 0; i < 6; i++) {
				auto size = discs.entries.at(i).size();
				if (size > biggestSlot) {
					biggestSlot = size;
					maxVarianceSlot = i;
				}
			}
		}

		std::optional<uint32_t> splitSlotRet = maxVarianceSlot;

		auto &chosenSplitSlot = discs.entries.at(maxVarianceSlot);
		auto midPoint = std::midpoint(size_t(0), chosenSplitSlot.size());
		auto copy1 = discs;
		auto copy2 = discs;
		auto &maxVarianceEntry = discs.entries.at(maxVarianceSlot);
		copy1.entries.at(maxVarianceSlot) = std::vector(maxVarianceEntry.begin(), std::next(maxVarianceEntry.begin(), static_cast<int32_t>(midPoint)));
		copy2.entries.at(maxVarianceSlot) = std::vector(std::next(maxVarianceEntry.begin(), static_cast<int32_t>(midPoint)), maxVarianceEntry.end());

		auto countsFirst = state.countSetsForDiscs(copy1.entries.at(maxVarianceSlot));
		auto countsSecond = std::array<uint32_t, 3>{
			state.sets[0].countsForSlot[maxVarianceSlot] - countsFirst[0],
			state.sets[1].countsForSlot[maxVarianceSlot] - countsFirst[1],
			state.sets[2].countsForSlot[maxVarianceSlot] - countsFirst[2]
		};
		{
			auto stateCopy1 = state;
			stateCopy1.setCountsForIndex(maxVarianceSlot, countsFirst);
			bnb(copy1, solutions, agent, ctx, node, stateCopy1, splitSlotRet);
		}
		{
			auto stateCopy2 = state;
			stateCopy2.setCountsForIndex(maxVarianceSlot, countsSecond);
			bnb(copy2, solutions, agent, ctx, node, stateCopy2, splitSlotRet);
		}
	}
}// namespace Optimization