#pragma once

#include "artifact/key.hpp"
#include "character/instance.hpp"
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
		::Artifact::InstanceKey artifactFlower;
		::Artifact::InstanceKey artifactPlume;
		::Artifact::InstanceKey artifactSands;
		::Artifact::InstanceKey artifactGoblet;
		::Artifact::InstanceKey artifactCirclet;
		std::vector<Serialization::Save::OptionTypes> options;
		std::vector<Serialization::Save::OptionTypes> artifactOptions;
		std::vector<Serialization::Save::Combo> combos;

		static Character fromInstance(const ::Character::Instance &);
		::Character::Instance toInstance() const;
	};
}// namespace Serialization::Save