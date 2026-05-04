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
		const std::optional<Stats::DiscBonus> &bonus1,
		const std::optional<Stats::DiscBonus> &bonus2,
		const std::optional<uint32_t> &splitSlot
	) {
		auto &loadout = agent.state.loadout();
		std::array<Stats::Sheet<float>, 6> statsForSlot = getMaxStatsForSlots(discs);

		// Check if this branch can possibly be good enough to be worth considering
		if (bonus1.has_value()) loadout.disc.bonus1.emplace(bonus1.value());
		if (bonus2.has_value()) loadout.disc.bonus2.emplace(bonus2.value());
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
				if (loadout.disc.bonus1.has_value()) {
					if (!bonus1.has_value()) continue;
					if (loadout.disc.bonus1.value().bonusPtr != bonus1.value().bonusPtr) continue;
				}
				if (loadout.disc.bonus2.has_value()) {
					if (!bonus2.has_value()) continue;
					if (loadout.disc.bonus2.value().bonusPtr != bonus2.value().bonusPtr) continue;
				}
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

		bnb(copy1, solutions, agent, ctx, node, bonus1, bonus2, splitSlotRet);
		bnb(copy2, solutions, agent, ctx, node, bonus1, bonus2, splitSlotRet);
	}
}// namespace Optimization