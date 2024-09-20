#pragma once

#include "cstdint"

namespace Artifact {
	enum class Slot : uint8_t {
		flower,
		plume,
		sands,
		goblet,
		circlet,
	};

	static inline std::vector<Slot> slots{
		Slot::flower,
		Slot::plume,
		Slot::sands,
		Slot::goblet,
		Slot::circlet,
	};
}// namespace Artifact