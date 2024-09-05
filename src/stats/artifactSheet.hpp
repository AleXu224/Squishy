#pragma once

#include "sheet.hpp"
#include "value.hpp"


namespace Stats {
	struct Loadout;
	struct ArtifactSheet {
		Stats::Sheet<Stats::Value<float, 7>> preMods{};
		Stats::Sheet<Stats::Value<float, 2>> postMods{};
		Stats::Sheet<Stats::Value<float, 2>> teamPreMods{};
		Stats::Sheet<Stats::Value<float, 2>> teamPostMods{};
		Stats::EnemySheet<Stats::Value<float, 2>> enemySheet{};

		Talents<Stats::Value<uint32_t, 2>> talents{};
		Talents<Stats::Value<uint32_t, 2>> teamTalents{};
	};
}// namespace Stats