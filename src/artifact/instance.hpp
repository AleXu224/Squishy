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
		std::array<std::optional<StatValue>, 4> subStats{};
		Level level{};
		uint8_t rarity = 5;
		Character::InstanceKey equippedCharacter{};

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
				if (subStat.has_value()) {
					const auto &val = subStat.value();
					stats.fromStat(val.stat) = val.value;
				}
			}
		}

		void unequip();
		void equipOn(Character::InstanceKey characterKey);
	};
}// namespace Artifact
