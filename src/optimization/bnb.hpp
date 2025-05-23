#pragma once

#include "character/instance.hpp"
#include "optimization/artifactFilter.hpp"
#include "solution.hpp"
#include <numeric>

namespace Optimization {
	static inline void bnb(
		FilteredArtifacts &artifacts,
		Solutions &solutions,
		Character::Instance &character,
		const Formula::Context &ctx,
		const Formula::Compiled::FloatNode &node,
		const std::optional<Stats::ArtifactBonus> &bonus1,
		const std::optional<Stats::ArtifactBonus> &bonus2,
		const std::optional<uint32_t> &splitSlot
	) {
		auto &loadout = character.state.loadout();
		std::array<Stats::Sheet<float>, 5> statsForSlot = getMaxStatsForSlots(artifacts);

		// Check if this branch can possibly be good enough to be worth considering
		if (bonus1.has_value()) loadout.artifact.bonus1.emplace(bonus1.value());
		if (bonus2.has_value()) loadout.artifact.bonus2.emplace(bonus2.value());
		loadout.artifact.sheet.equippedArtifacts[0] = &statsForSlot[0];
		loadout.artifact.sheet.equippedArtifacts[1] = &statsForSlot[1];
		loadout.artifact.sheet.equippedArtifacts[2] = &statsForSlot[2];
		loadout.artifact.sheet.equippedArtifacts[3] = &statsForSlot[3];
		loadout.artifact.sheet.equippedArtifacts[4] = &statsForSlot[4];
		if (node.eval(ctx) <= solutions.minScore) return;

		float maxVariance = 0;
		size_t maxVarianceSlot = 0;

		// Check the potential of each artifact
		for (size_t i = 0; i < 5; i++) {
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
			auto &artis = artifacts.entries.at(i);
			float maxVal = 0;
			float minVal = std::numeric_limits<float>::max();

			artis.erase(
				std::remove_if(artis.begin(), artis.end(), [&](Artifact::Instance *artifact) {
					loadout.artifact.sheet.equippedArtifacts.at(i) = &artifact->stats;

					auto val = node.eval(ctx);
					if (val > solutions.minScore) {
						maxVal = std::max(val, maxVal);
						minVal = std::min(val, minVal);
					}

					return val <= solutions.minScore;
				}),
				artis.end()
			);
			loadout.artifact.sheet.equippedArtifacts.at(i) = &statsForSlot.at(i);

			auto variance = maxVal - minVal;
			varianceFunc(variance);
		}

		// Bruteforce if the combination count is low enough
		if (artifacts.getCombCount() <= 32) {
			auto &equipped = std::get<Stats::Artifact::Slotted>(loadout.artifact.equipped);
			for (const auto &[flower, plume, sands, goblet, circlet]: std::views::cartesian_product(artifacts.entries.at(0), artifacts.entries.at(1), artifacts.entries.at(2), artifacts.entries.at(3), artifacts.entries.at(4))) {
				equipped.flower = flower->key;
				equipped.plume = plume->key;
				equipped.sands = sands->key;
				equipped.goblet = goblet->key;
				equipped.circlet = circlet->key;
				loadout.artifact.refreshStats();
				if (loadout.artifact.bonus1.has_value()) {
					if (!bonus1.has_value()) continue;
					if (loadout.artifact.bonus1.value().bonusPtr != bonus1.value().bonusPtr) continue;
				}
				if (loadout.artifact.bonus2.has_value()) {
					if (!bonus2.has_value()) continue;
					if (loadout.artifact.bonus2.value().bonusPtr != bonus2.value().bonusPtr) continue;
				}
				auto dmg = node.eval(ctx);
				solutions.addSolution(equipped, dmg);
			}
			return;
		}

		size_t biggestSlot = 0;
		// size_t biggestSlotIndex = 0;
		if (maxVariance == 0) {
			for (size_t i = 0; i < 5; i++) {
				auto size = artifacts.entries.at(i).size();
				if (size > biggestSlot) {
					biggestSlot = size;
					maxVarianceSlot = i;
				}
			}
		}

		std::optional<uint32_t> splitSlotRet = maxVarianceSlot;

		auto &chosenSplitSlot = artifacts.entries.at(maxVarianceSlot);
		auto midPoint = std::midpoint(size_t(0), chosenSplitSlot.size());
		auto copy1 = artifacts;
		auto copy2 = artifacts;
		auto &maxVarianceEntry = artifacts.entries.at(maxVarianceSlot);
		copy1.entries.at(maxVarianceSlot) = std::vector(maxVarianceEntry.begin(), std::next(maxVarianceEntry.begin(), static_cast<int32_t>(midPoint)));
		copy2.entries.at(maxVarianceSlot) = std::vector(std::next(maxVarianceEntry.begin(), static_cast<int32_t>(midPoint)), maxVarianceEntry.end());

		bnb(copy1, solutions, character, ctx, node, bonus1, bonus2, splitSlotRet);
		bnb(copy2, solutions, character, ctx, node, bonus1, bonus2, splitSlotRet);
	}
}// namespace Optimization