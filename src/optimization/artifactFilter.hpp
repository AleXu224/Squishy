#pragma once

#include "artifact/instance.hpp"
#include "artifact/key.hpp"
#include "artifact/slot.hpp"
#include "ranges"
#include "stats/artifact.hpp"
#include <optional>


namespace Optimization {
	struct FilteredArtifacts {
		std::array<std::vector<Artifact::Instance *>, 5> entries{};

		size_t getCombCount() const {
			return entries[0].size()
				 * entries[1].size()
				 * entries[2].size()
				 * entries[3].size()
				 * entries[4].size();
		}
	};

	struct ArtifactSlotFilter {
		Artifact::Slot slot;
		std::optional<Artifact::SetKey> set{};
		std::optional<Artifact::SetKey> notSet{};

		[[nodiscard]] bool eval(const Artifact::Instance &instance) const {
			if (instance.slot != slot) return false;
			if (set && instance.set != set) return false;
			if (notSet && instance.set == notSet) return false;

			return true;
		}
	};

	struct ArtifactFilter {
		std::array<ArtifactSlotFilter, 5> filters{
			ArtifactSlotFilter{.slot = Artifact::Slot::flower},
			ArtifactSlotFilter{.slot = Artifact::Slot::plume},
			ArtifactSlotFilter{.slot = Artifact::Slot::sands},
			ArtifactSlotFilter{.slot = Artifact::Slot::goblet},
			ArtifactSlotFilter{.slot = Artifact::Slot::circlet},
		};
		std::optional<Stats::ArtifactBonus> bonus1;
		std::optional<Stats::ArtifactBonus> bonus2;

		FilteredArtifacts filter(std::vector<Artifact::Instance> &artifacts) const {
			FilteredArtifacts ret{};
			for (auto [dst, filter]: std::views::zip(ret.entries, filters)) {
				for (auto &artifact: artifacts) {
					if (filter.eval(artifact)) dst.emplace_back(&artifact);
				}
			}

			return ret;
		}
		FilteredArtifacts filter(FilteredArtifacts &artifacts) const {
			FilteredArtifacts ret{};
			for (auto [artifacts, dst, filter]: std::views::zip(artifacts.entries, ret.entries, filters)) {
				for (auto &artifact: artifacts) {
					if (filter.eval(*artifact)) dst.emplace_back(artifact);
				}
			}

			return ret;
		}
	};

	// Combines all the artifacts of a slot into one unrealistically optimistic one to get the max potential
	static inline std::array<Stats::Sheet<float>, 5> getMaxStatsForSlots(auto &&artifacts) {
		std::array<Stats::Sheet<float>, 5> statsForSlot{};

		for (auto &&[slot, artifacts]: std::views::zip(statsForSlot, artifacts.entries)) {
			for (const auto &artifact: artifacts) {
				auto &mainStatSlot = slot.fromStat(artifact->mainStat);
				auto &mainStatArtifact = artifact->stats.fromStat(artifact->mainStat);
				mainStatSlot = std::max(mainStatSlot, mainStatArtifact);

				for (const auto &subStat: artifact->subStats) {
					if (!subStat.has_value()) continue;
					auto &statSlot = slot.fromStat(subStat->stat);
					auto &statArtifact = artifact->stats.fromStat(subStat->stat);
					statSlot = std::max(statSlot, statArtifact);
				}
			}
		}
		return statsForSlot;
	}
}// namespace Optimization