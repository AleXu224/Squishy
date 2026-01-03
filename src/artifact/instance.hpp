#pragma once

#include "artifact/key.hpp"
#include "artifact/slot.hpp"
#include "character/key.hpp"
#include "observer.hpp"
#include "stats/sheet.hpp"
#include "stats/stat.hpp"
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
		std::array<StatValue, 4> subStats{};
		Level level{};
		uint8_t rarity = 5;

		Stats::Sheet<float> stats{};

		squi::VoidObservable updateEvent{};

		void updateStats() {
			// Reset the stats
			// It's easier to to just recreate the entire thing that to go through each stat
			stats = Stats::Sheet<float>();

			// Main stat
			stats.fromStat(mainStat) = Stats::Values::mainStat.at(mainStat).at(rarity - 1).at(level);

			// Sub stats
			for (const auto &subStat: subStats) {
				if (subStat.stat.has_value() && subStat.activated) {
					stats.fromStat(subStat.stat.value()) = subStat.value;
				}
			}
		}

		void unequip() const;
		void equipOn(Character::InstanceKey characterKey, std::optional<uint32_t> loadout = std::nullopt) const;
		// Characters that have this artifact equipped
		[[nodiscard]] Character::InstanceKey equippedOn() const;
		// Characters that have this artifact in a build
		[[nodiscard]] std::vector<Character::InstanceKey> usedOn() const;

		void refreshUsages() const;
	};
}// namespace Artifact
