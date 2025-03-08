#pragma once

#include "artifact/key.hpp"
#include "character/key.hpp"
#include "combo.hpp"
#include "option.hpp"
#include "weapon/key.hpp"


namespace Serialization::Save {
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
		std::vector<Serialization::Save::OptionTypes> artifactOptions;
		std::vector<Serialization::Save::Combo> combos;
	};
}// namespace Serialization::Save