#pragma once

#include "cstdint"
#include <vector>

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

	enum class SetSlot : uint8_t {
		twoPiece,
		fourPiece,
	};

	static inline std::vector<SetSlot> setSlots{
		SetSlot::twoPiece,
		SetSlot::fourPiece,
	};
}// namespace Artifact