#pragma once

#include "artifact/key.hpp"
#include "option/option.hpp"
#include "stats/artifactSheet.hpp"


namespace Artifact {
	struct Set;
}

namespace Stats {
	struct Artifact {
		// The 4 piece set equipped if it exists
		std::optional<std::reference_wrapper<const ::Artifact::Set>> set{};
		std::optional<std::reference_wrapper<Option::ArtifactMap>> currentOptions{};
		std::unordered_map<::Artifact::SetKey, Option::ArtifactMap> options{};
		ArtifactSheet sheet{};

		struct Slotted {
			std::optional<::Artifact::InstanceKey> flower;
			std::optional<::Artifact::InstanceKey> plume;
			std::optional<::Artifact::InstanceKey> sands;
			std::optional<::Artifact::InstanceKey> goblet;
			std::optional<::Artifact::InstanceKey> circlet;

			[[nodiscard]] static inline auto getMembers() {
				return std::array{
					&Slotted::flower,
					&Slotted::plume,
					&Slotted::sands,
					&Slotted::goblet,
					&Slotted::circlet,
				};
			}
		};

		Slotted equipped{};
	};
}// namespace Stats