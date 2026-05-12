#pragma once

#include "skill.hpp"
#include "stats/sheet.hpp"
#include "stats/value.hpp"

namespace Stats {
	struct Loadout;
	struct State;
	struct AgentSheet {
		uint8_t mindscape{};
		uint8_t level{1};
		uint8_t promotion{0};

		// Promotion and base
		Stats::Sheet<Stats::Value<float, 2>> base{
			.er{.constant = 1.2f},
			.cr{.constant = 0.05f},
			.cd{.constant = 0.5f},
		};
		Stats::Sheet<Stats::Value<float, 1>> initial{};
		Stats::Sheet<Stats::Value<float, 1>> combat{};
		Skills<Stats::Value<int32_t, 2>> skills{};

		void init(Stats::State &stats);
	};
}// namespace Stats
