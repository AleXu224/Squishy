#pragma once

#include "artifact/key.hpp"
#include "cereal/cereal.hpp"
#include "cereal/types/vector.hpp"// IWYU pragma: export
#include "character/key.hpp"
#include "option.hpp"
#include "weapon/key.hpp"


namespace Serialization::Save {
	struct ArtifactOptions {
		::Artifact::SetKey setKey;
		std::vector<Serialization::Save::OptionTypes> options;

		template<class Archive>
		void serialize(Archive &ar) {
			ar(
				CEREAL_NVP(setKey),
				CEREAL_NVP(options)
			);
		}
	};

	struct Character {
		::Character::InstanceKey instanceKey;
		::Character::DataKey dataKey;
		::Weapon::InstanceKey weaponInstanceKey;
		uint8_t constellation;
		uint8_t level;
		uint8_t ascension;
		uint32_t normalLevel;
		uint32_t skillLevel;
		uint32_t burstLevel;
		std::optional<::Artifact::InstanceKey> artifactFlower;
		std::optional<::Artifact::InstanceKey> artifactPlume;
		std::optional<::Artifact::InstanceKey> artifactSands;
		std::optional<::Artifact::InstanceKey> artifactGoblet;
		std::optional<::Artifact::InstanceKey> artifactCirclet;
		std::vector<Serialization::Save::OptionTypes> options;
		std::vector<ArtifactOptions> artifactOptions;

		template<class Archive>
		void serialize(Archive &ar) {
			ar(
				CEREAL_NVP(instanceKey),
				CEREAL_NVP(dataKey),
				CEREAL_NVP(weaponInstanceKey),
				CEREAL_NVP(constellation),
				CEREAL_NVP(level),
				CEREAL_NVP(ascension),
				CEREAL_NVP(normalLevel),
				CEREAL_NVP(skillLevel),
				CEREAL_NVP(burstLevel),
				CEREAL_NVP(artifactFlower),
				CEREAL_NVP(artifactPlume),
				CEREAL_NVP(artifactSands),
				CEREAL_NVP(artifactGoblet),
				CEREAL_NVP(artifactCirclet),
				CEREAL_NVP(options),
				CEREAL_NVP(artifactOptions)
			);
		}
	};
}// namespace Serialization::Save