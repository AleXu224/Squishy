#pragma once

#include "sheet.hpp"
#include "value.hpp"
#include <optional>


namespace Stats {
	struct Loadout;
	struct ArtifactSheet {
		Stats::Sheet<Stats::Value<float, 2>> preMods{};
		Stats::Sheet<Stats::Value<float, 1>> postMods{};
		Stats::Sheet<Stats::Value<float, 1>> teamPreMods{};
		Stats::Sheet<Stats::Value<float, 1>> teamPostMods{};
		Stats::EnemySheet<Stats::Value<float, 1>> enemySheet{};

		Talents<Stats::Value<uint32_t, 1>> talents{};
		Talents<Stats::Value<uint32_t, 1>> teamTalents{};

		std::array<std::optional<Stats::Sheet<float> *>, 5> equippedArtifacts{};

		void init();
	};
}// namespace Stats