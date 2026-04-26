#pragma once

#include "agent/key.hpp"
#include "disc/key.hpp"
#include "disc/slot.hpp"
#include "observer.hpp"
#include "stats/sheet.hpp"
#include "stats/stat.hpp"
#include "utils/stringify.hpp"// IWYU pragma: keep


#include <array>
#include <optional>

namespace Disc {
	using Level = uint8_t;

	struct Instance {
		InstanceKey key{};
		SetKey set{};
		Partition partition{};
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
			stats.fromStat(mainStat) = Stats::Values::mainStat.at(mainStat).at(rarity, level);

			// Sub stats
			for (const auto &subStat: subStats) {
				if (subStat.stat.has_value() && subStat.activated) {
					stats.fromStat(subStat.stat.value()) = subStat.value;
				}
			}
		}

		void unequip() const;
		void equipOn(Agent::InstanceKey agentKey, std::optional<uint32_t> loadout = std::nullopt) const;
		// Agents that have this disc equipped
		[[nodiscard]] Agent::InstanceKey equippedOn() const;
		// Agents that have this disc in a build
		[[nodiscard]] std::vector<Agent::InstanceKey> usedOn() const;

		void refreshUsages() const;
	};
}// namespace Disc
