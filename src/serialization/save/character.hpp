#pragma once

#include "character/instance.hpp"
#include "character/key.hpp"
#include "combo.hpp"
#include "optimization.hpp"
#include "option.hpp"
#include "loadout.hpp"

namespace Serialization::Save {
	struct Character {
		::Character::InstanceKey instanceKey;
		::Character::DataKey dataKey;
		uint8_t constellation;
		uint8_t level;
		uint8_t ascension;
		int32_t normalLevel;
		int32_t skillLevel;
		int32_t burstLevel;
		std::optional<uint32_t> loadoutIndex;
		Serialization::Save::Loadout equippedLoadout;
		std::vector<Serialization::Save::OptionTypes> options;
		std::vector<Serialization::Save::Combo> combos;
		std::vector<Serialization::Save::Loadout> loadouts;
		Optimization optimizationOptions;

		static Character fromInstance(const ::Character::Instance &);
		::Character::Instance toInstance() const;
	};
}// namespace Serialization::Save