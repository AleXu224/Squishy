#pragma once

#include "artifact/slot.hpp"
#include "string"
#include "unordered_map"


namespace Serialization::Good {
	static inline std::unordered_map<std::string, Artifact::Slot> slotKey{
		{"flower", Artifact::Slot::flower},
		{"plume", Artifact::Slot::plume},
		{"sands", Artifact::Slot::sands},
		{"goblet", Artifact::Slot::goblet},
		{"circlet", Artifact::Slot::circlet},
	};
}