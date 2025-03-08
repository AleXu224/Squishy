#pragma once

#include "option.hpp"
#include "weapon/key.hpp"

namespace Serialization::Save {
	struct Weapon {
		::Weapon::InstanceKey instanceKey;
		::Weapon::DataKey dataKey;
		uint8_t level;
		uint8_t ascension;
		uint8_t refinement;
		std::vector<Serialization::Save::OptionTypes> options;
	};
}// namespace Serialization::Save