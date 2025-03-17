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
		const Formula::FloatNode &node,
		const std::optional<Stats::ArtifactBonus> &bonus1,
		const std::optional<Stats::ArtifactBonus> &bonus2
	) {
		std::array<Stats::Sheet<float>, 5> statsForSlot = getMaxStatsForSlots(artifacts);

		// Check if this branch can possibly be good enough to be worth considering
		if (bonus1.has_value()) character.loadout.artifact.bonus1.emplace(bonus1.value());
		if (bonus2.has_value()) character.loadout.artifact.bonus2.emplace(bonus2.value());
		character.loadout.artifact.sheet.equippedArtifacts[0] = &statsForSlot[0];
		character.loadout.artifact.sheet.equippedArtifacts[1] = &statsForSlot[1];
		character.loadout.artifact.sheet.equippedArtifacts[2] = &statsForSlot[2];
		character.loadout.artifact.sheet.equippedArtifacts[3] = &statsForSlot[3];
		character.loadout.artifact.sheet.equippedArtifacts[4] = &statsForSlot[4];
		if (node.eval(ctx) <= solutions.minScore) return;

		float maxVariance = 0;
		size_t maxVarianceSlot = 0;

		// Check the potential of each artifact
		for (size_t i = 0; i < 5; i++) {
			auto &artis = artifacts.entries[i];
			float maxVal = 0;
			float minVal = std::numeric_limits<float>::max();

			artis.erase(
				std::remove_if(artis.begin(), artis.end(), [&](Artifact::Instance *artifact) {
					character.loadout.artifact.sheet.equippedArtifacts[i] = &artifact->stats;

					auto val = node.eval(ctx);
					if (val > solutions.minScore) {
						maxVal = std::max(val, maxVal);
						minVal = std::min(val, minVal);
					}

					return val <= solutions.minScore;
				}),
				artis.end()
			);
			character.loadout.artifact.sheet.equippedArtifacts[i] = &statsForSlot[i];

			auto variance = maxVal - minVal;
			if (variance > maxVariance) {
				maxVariance = variance;
				maxVarianceSlot = i;
			}
		}

		// Bruteforce if the combination count is low enough
		if (artifacts.getCombCount() <= 32) {
			for (const auto &[flower, plume, sands, goblet, circlet]: std::views::cartesian_product(artifacts.entries.at(0), artifacts.entries.at(1), artifacts.entries.at(2), artifacts.entries.at(3), artifacts.entries.at(4))) {
				character.loadout.artifact.equipped.flower = flower->key;
				character.loadout.artifact.equipped.plume = plume->key;
				character.loadout.artifact.equipped.sands = sands->key;
				character.loadout.artifact.equipped.goblet = goblet->key;
				character.loadout.artifact.equipped.circlet = circlet->key;
				character.loadout.artifact.refreshStats();
				if (character.loadout.artifact.bonus1.has_value()) {
					if (!bonus1.has_value()) continue;
					if (&character.loadout.artifact.bonus1.value().bonusPtr != &bonus1.value().bonusPtr) continue;
				}
				if (character.loadout.artifact.bonus2.has_value()) {
					if (!bonus2.has_value()) continue;
					if (&character.loadout.artifact.bonus2.value().bonusPtr != &bonus2.value().bonusPtr) continue;
				}
				auto dmg = node.eval(ctx);
				solutions.addSolution(character.loadout.artifact.equipped, dmg);
			}
			return;
		}

		size_t biggestSlot = 0;
		// size_t biggestSlotIndex = 0;
		if (maxVariance == 0) {
			for (size_t i = 0; i < 5; i++) {
				auto size = artifacts.entries[i].size();
				if (size > biggestSlot) {
					biggestSlot = size;
					maxVarianceSlot = i;
				}
			}
		}

		auto &chosenSplitSlot = artifacts.entries[maxVarianceSlot];
		auto midPoint = std::midpoint(size_t(0), chosenSplitSlot.size());
		auto copy1 = artifacts;
		auto copy2 = artifacts;
		auto &maxVarianceEntry = artifacts.entries[maxVarianceSlot];
		copy1.entries[maxVarianceSlot] = std::vector(maxVarianceEntry.begin(), maxVarianceEntry.begin() + midPoint);
		copy2.entries[maxVarianceSlot] = std::vector(maxVarianceEntry.begin() + midPoint, maxVarianceEntry.end());

		bnb(copy1, solutions, character, ctx, node, bonus1, bonus2);
		bnb(copy2, solutions, character, ctx, node, bonus1, bonus2);
	}
}// namespace Optimization