#pragma once

#include "artifact/key.hpp"
#include "conditional/conditional.hpp"
#include "stats/artifactSheet.hpp"


namespace Artifact {
	struct Set;
}

namespace Stats {
	struct Artifact {
		// The 4 piece set equipped if it exists
		std::optional<std::reference_wrapper<const ::Artifact::Set>> set{};
		Conditional::ArtifactMap conditionals{};
		ArtifactSheet sheet{};

		struct Slotted {
			::Artifact::Key flower;
			::Artifact::Key plume;
			::Artifact::Key sands;
			::Artifact::Key goblet;
			::Artifact::Key circlet;

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