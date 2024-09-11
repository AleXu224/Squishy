#pragma once

#include "Rarity.hpp"
#include "artifact/key.hpp"
#include "artifact/slot.hpp"
#include "cereal/cereal.hpp"
#include "character/key.hpp"
#include "stats/stat.hpp"
#include <optional>


#include "cereal/types/array.hpp"   // IWYU pragma: export
#include "cereal/types/optional.hpp"// IWYU pragma: export

namespace Serialization::Save {
	struct ArtifactSubStat {
		::Stat stat;
		float value;

		template<class Archive>
		void serialize(Archive &ar) {
			ar(
				CEREAL_NVP(stat),
				CEREAL_NVP(value)
			);
		}
	};

	struct Artifact {
		::Artifact::InstanceKey instanceKey{};
		::Artifact::SetKey setKey{};
		::Artifact::Slot slot = ::Artifact::Slot::flower;
		::Stat mainStat = Stat::hp;
		std::array<std::optional<ArtifactSubStat>, 4> subStats{};
		uint8_t level{};
		::Rarity rarity = Rarity::fiveStar;
		::Character::InstanceKey equippedCharacter{};

		template<class Archive>
		void serialize(Archive &ar) {
			ar(
				CEREAL_NVP(instanceKey),
				CEREAL_NVP(setKey),
				CEREAL_NVP(slot),
				CEREAL_NVP(mainStat),
				CEREAL_NVP(subStats),
				CEREAL_NVP(level),
				CEREAL_NVP(rarity),
				CEREAL_NVP(equippedCharacter)
			);
		}
	};
}// namespace Serialization::Save