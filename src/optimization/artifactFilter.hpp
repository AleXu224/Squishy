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

		FilteredArtifacts getTop(size_t count) {
			std::partial_sort(entries.at(0).begin(), std::next(entries.at(0).begin(), std::min(count, entries.at(0).size())), entries.at(0).end(), std::greater<ScoredArtifact>{});
			std::partial_sort(entries.at(1).begin(), std::next(entries.at(1).begin(), std::min(count, entries.at(1).size())), entries.at(1).end(), std::greater<ScoredArtifact>{});
			std::partial_sort(entries.at(2).begin(), std::next(entries.at(2).begin(), std::min(count, entries.at(2).size())), entries.at(2).end(), std::greater<ScoredArtifact>{});
			std::partial_sort(entries.at(3).begin(), std::next(entries.at(3).begin(), std::min(count, entries.at(3).size())), entries.at(3).end(), std::greater<ScoredArtifact>{});
			std::partial_sort(entries.at(4).begin(), std::next(entries.at(4).begin(), std::min(count, entries.at(4).size())), entries.at(4).end(), std::greater<ScoredArtifact>{});
			return {
				.entries{
					std::vector<ScoredArtifact>(entries.at(0).begin(), std::next(entries.at(0).begin(), std::min(count, entries.at(0).size()))),
					std::vector<ScoredArtifact>(entries.at(1).begin(), std::next(entries.at(1).begin(), std::min(count, entries.at(1).size()))),
					std::vector<ScoredArtifact>(entries.at(2).begin(), std::next(entries.at(2).begin(), std::min(count, entries.at(2).size()))),
					std::vector<ScoredArtifact>(entries.at(3).begin(), std::next(entries.at(3).begin(), std::min(count, entries.at(3).size()))),
					std::vector<ScoredArtifact>(entries.at(4).begin(), std::next(entries.at(4).begin(), std::min(count, entries.at(4).size()))),
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