#pragma once

#include "artifact/instance.hpp"
#include "artifact/key.hpp"
#include "artifact/slot.hpp"
#include <optional>


namespace Optimization {
	struct ArtifactSlotFilter {
		Artifact::Slot slot;
		std::optional<Artifact::SetKey> set{};

		[[nodiscard]] bool eval(const Artifact::Instance &instance) {
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
	};
}// namespace Optimization