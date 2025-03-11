#pragma once

#include "artifact/instance.hpp"
#include "artifact/key.hpp"
#include "artifact/slot.hpp"
#include "ranges"
#include <optional>


namespace Optimization {
	struct FilteredArtifacts {
		std::array<std::vector<Artifact::Instance *>, 5> entries{};
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