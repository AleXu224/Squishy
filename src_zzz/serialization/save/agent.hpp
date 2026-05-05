#pragma once

#include "agent/instance.hpp"
#include "agent/key.hpp"
#include "combo.hpp"
#include "loadout.hpp"
#include "optimization.hpp"
#include "option.hpp"


namespace Serialization::Save {
	struct Agent {
		::Agent::InstanceKey instanceKey;
		::Agent::DataKey dataKey;
		uint8_t mindscape;
		uint8_t level;
		uint8_t promotion;
		int32_t basicLevel;
		int32_t dodgeLevel;
		int32_t assistLevel;
		int32_t specialLevel;
		int32_t chainLevel;
		int32_t coreLevel;
		std::optional<uint32_t> loadoutIndex;
		Serialization::Save::Loadout equippedLoadout;
		std::vector<Serialization::Save::OptionTypes> options;
		std::vector<Serialization::Save::Combo> combos;
		std::vector<Serialization::Save::Loadout> loadouts;
		Optimization optimizationOptions;

		static Agent fromInstance(const ::Agent::Instance &);
		::Agent::Instance toInstance() const;
	};
}// namespace Serialization::Save