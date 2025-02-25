#pragma once

#include "Talents.hpp"
#include "stats/sheet.hpp"
#include "stats/value.hpp"

namespace Stats {
	struct Loadout;
	struct CharacterSheet {
		Formula::ElementNode infusion;
		Formula::ElementNode teamInfusion;

		uint8_t constellation{};
		uint8_t level{1};
		uint8_t ascension{0};

		// Ascension and base
		Stats::Sheet<Stats::Value<float, 2>> stats{
			.er{.constant = 1.f},
			.cr{.constant = 0.05f},
			.cd{.constant = 0.5f},
		};
		Talents<Stats::Value<uint32_t, 1>> talents{};

		void init(Stats::Loadout &stats);
	};
}// namespace Stats
