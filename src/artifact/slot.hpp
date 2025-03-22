#pragma once

#include "cstdint"
#include "utility"
#include "utils/stringify.hpp"
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


namespace Utils {
	template<>
	inline std::string Stringify<>(const Artifact::Slot &slot) {
		switch (slot) {
			case Artifact::Slot::flower:
				return "Flower";
			case Artifact::Slot::plume:
				return "Plume";
			case Artifact::Slot::sands:
				return "Sands";
			case Artifact::Slot::goblet:
				return "Goblet";
			case Artifact::Slot::circlet:
				return "Circlet";
		}
		std::unreachable();
	}
}// namespace Utils