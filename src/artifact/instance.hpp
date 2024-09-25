#pragma once

#include "Rarity.hpp"
#include "artifact/key.hpp"
#include "artifact/slot.hpp"
#include "character/key.hpp"
#include "observer.hpp"
#include "stats/sheet.hpp"
#include "stats/stat.hpp"
#include "utility"
#include "utils/stringify.hpp"// IWYU pragma: keep


#include <array>
#include <optional>

namespace Artifact {
	using Level = uint8_t;

	struct Instance {
		InstanceKey key{};
		SetKey set{};
		Slot slot{};
		Stat mainStat{};
		std::array<std::optional<StatValue>, 4> subStats;
		Level level{};
		Rarity rarity{};
		Character::InstanceKey equippedCharacter{};

		Stats::Sheet<float> stats{};

		squi::VoidObservable updateEvent{};

		void updateStats() {
			// Reset the stats
			// It's easier to to just recreate the entire thing that to go through each stat
			stats = Stats::Sheet<float>();

			// Main stat
			stats.fromStat(mainStat) = Stats::Values::mainStat.at(mainStat).at(level);

			// Sub stats
			for (const auto &subStat: subStats) {
				if (subStat.has_value()) {
					const auto &val = subStat.value();
					stats.fromStat(val.stat) = val.value;
				}
			}
		}
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
