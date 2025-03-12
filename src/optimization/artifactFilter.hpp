#pragma once

#include "artifact/instance.hpp"
#include "artifact/key.hpp"
#include "artifact/slot.hpp"
#include "ranges"
#include <algorithm>
#include <optional>


namespace Optimization {
	struct ScoredArtifact {
		Artifact::Instance *instance;
		float maxScore = 0;

		auto operator<=>(this auto &&self, const ScoredArtifact &other) {
			return self.maxScore <=> other.maxScore;
		}
	};

	struct FilteredArtifacts {
		std::array<std::vector<ScoredArtifact>, 5> entries{};

		FilteredArtifacts getTop() {
			std::partial_sort(entries.at(0).begin(), entries.at(0).begin() + 5, entries.at(0).end(), std::greater<ScoredArtifact>{});
			std::partial_sort(entries.at(1).begin(), entries.at(1).begin() + 5, entries.at(1).end(), std::greater<ScoredArtifact>{});
			std::partial_sort(entries.at(2).begin(), entries.at(2).begin() + 5, entries.at(2).end(), std::greater<ScoredArtifact>{});
			std::partial_sort(entries.at(3).begin(), entries.at(3).begin() + 5, entries.at(3).end(), std::greater<ScoredArtifact>{});
			std::partial_sort(entries.at(4).begin(), entries.at(4).begin() + 5, entries.at(4).end(), std::greater<ScoredArtifact>{});
			return {
				.entries{
					std::vector<ScoredArtifact>(entries.at(0).begin(), entries.at(0).begin() + 5),
					std::vector<ScoredArtifact>(entries.at(1).begin(), entries.at(1).begin() + 5),
					std::vector<ScoredArtifact>(entries.at(2).begin(), entries.at(2).begin() + 5),
					std::vector<ScoredArtifact>(entries.at(3).begin(), entries.at(3).begin() + 5),
					std::vector<ScoredArtifact>(entries.at(4).begin(), entries.at(4).begin() + 5),
				},
			};
		}
	};

	struct ArtifactSlotFilter {
		Artifact::Slot slot;
		std::optional<Artifact::SetKey> set{};

		[[nodiscard]] bool eval(const Artifact::Instance &instance) const {
			if (instance.slot != slot) return false;
			if (set && instance.set != set) return false;

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

		FilteredArtifacts filter(std::vector<Artifact::Instance> &artifacts) const {
			FilteredArtifacts ret{};
			for (auto [dst, filter]: std::views::zip(ret.entries, filters)) {
				for (auto &artifact: artifacts) {
					if (filter.eval(artifact)) dst.emplace_back(&artifact);
				}
			}

			return ret;
		}
	};
}// namespace Optimization