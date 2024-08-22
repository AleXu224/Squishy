#pragma once

#include "sheet.hpp"
#include "value.hpp"


namespace Stats {
	struct Loadout;
	struct ArtifactSheet {
		Stats::Sheet<Stats::Value<Loadout, Team, 7>> preMods{};
		Stats::Sheet<Stats::Value<Loadout, Team, 2>> postMods{};
		Stats::Sheet<Stats::Value<Loadout, Team, 2>> teamPreMods{};
		Stats::Sheet<Stats::Value<Loadout, Team, 2>> teamPostMods{};
	};
}// namespace Stats