#pragma once

#include "sheet.hpp"
#include "value.hpp"


namespace Stats {
	struct Loadout;
	struct ArtifactSheet {
		Stats::Sheet<Stats::Value<Formula::Context, 7>> preMods{};
		Stats::Sheet<Stats::Value<Formula::Context, 2>> postMods{};
		Stats::Sheet<Stats::Value<Formula::Context, 2>> teamPreMods{};
		Stats::Sheet<Stats::Value<Formula::Context, 2>> teamPostMods{};
		Stats::EnemySheet<Stats::Value<Formula::Context, 2>> enemySheet{};
	};
}// namespace Stats