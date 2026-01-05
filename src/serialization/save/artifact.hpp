#pragma once

#include "artifact/instance.hpp"
#include "artifact/key.hpp"
#include "artifact/slot.hpp"
#include "stats/stat.hpp"
#include <optional>


namespace Serialization::Save {
	struct ArtifactSubStat {
		::Stat stat;
		bool activated = true;
		float value;
	};

	struct Artifact {
		::Artifact::InstanceKey instanceKey{};
		::Artifact::SetKey setKey{};
		::Artifact::Slot slot = ::Artifact::Slot::flower;
		::Stat mainStat = Stat::hp;
		std::array<std::optional<ArtifactSubStat>, 4> subStats{};
		uint8_t level{};
		uint8_t rarity = 5;

		static Artifact fromInstance(const ::Artifact::Instance &);
		::Artifact::Instance toInstance() const;
	};
}// namespace Serialization::Save