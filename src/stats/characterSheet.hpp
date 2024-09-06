#pragma once

#include "Talents.hpp"
#include "stats/sheet.hpp"
#include "stats/value.hpp"

namespace Stats {
	struct Loadout;
	struct CharacterSheet {
		// Idx 0: kit, Idx 1: weapon and artifact, Idx 2: ascension and base, Idx 3: team mods
		Stats::Sheet<Stats::Value<float, 4>> preMods{
			.er{.constant = 1.f},
			.cr{.constant = 0.05f},
			.cd{.constant = 0.5f},
		};
		// Idx 0: kit, Idx 1: weapon and artifact, Idx 2: preMods, Idx 3: team mods
		Stats::Sheet<Stats::Value<float, 4>> postMods{};
		// Idx 0: kit, Idx 1: weapon and artifact
		Stats::Sheet<Stats::Value<float, 2>> teamPreMods{};
		Stats::Sheet<Stats::Value<float, 2>> teamPostMods{};
		Stats::EnemySheet<Stats::Value<float, 2>> enemySheet{};

		Formula::ElementNode infusion;
		Formula::ElementNode teamInfusion;

		// Idx 0: kit, Idx 1: weapon and artifact, Idx 2: Constellation 3 and 5, Idx 3: team
		using _Talents = Talents<Stats::Value<uint32_t, 4>>;
		_Talents talents{};
		// Idx 0: kit, Idx 1: weapon and artifact
		using _TeamTalents = Talents<Stats::Value<uint32_t, 2>>;
		_TeamTalents teamTalents{};

		uint8_t constellation{};
		uint8_t level{1};
		uint8_t ascension{0};

		void init(Stats::Loadout &stats);

	private:
		void linkWeaponAndArtifacts();
	};
}// namespace Stats
