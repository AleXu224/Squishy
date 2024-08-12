#pragma once

#include "Rarity.hpp"
#include "artifact/key.hpp"
#include "character/key.hpp"
#include "stats/stat.hpp"
#include "utility"
#include "utils/stringify.hpp"// IWYU pragma: keep


#include <array>

namespace Artifact {
	using SetKey = uint16_t;
	using Level = uint8_t;

	enum class Slot {
		flower,
		plume,
		sands,
		goblet,
		circlet,
	};

	struct Instance {
		Key key;
		SetKey set;
		Slot slot;
		Stat mainStat;
		std::array<StatValue, 4> subStats;
		Level level;
		Rarity rarity;
		Character::Key equippedCharacter;
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

// static const std::unordered_map<ArtifactSlot, std::string_view> artifactSlotToIcon = {
// 	{ArtifactSlot::flower, "\u0052"},
// 	{ArtifactSlot::plume, "\u0053"},
// 	{ArtifactSlot::sands, "\u0054"},
// 	{ArtifactSlot::goblet, "\u0055"},
// 	{ArtifactSlot::circlet, "\u0056"},
// };
