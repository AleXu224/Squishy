#pragma once

#include "Talents.hpp"
#include "stats/sheet.hpp"
#include "stats/value.hpp"

namespace Stats {
	struct Loadout;
	struct CharacterSheet {
		// Idx 0: kit, Idx 1: weapon and artifact, Idx 2: ascension and base, Idx 3: team mods
		Stats::Sheet<Stats::Value<Formula::Context, 4>> preMods{
			.er{.constant = 1.f},
			.cr{.constant = 0.05f},
			.cd{.constant = 0.5f},
		};
		// Idx 0: kit, Idx 1: weapon and artifact, Idx 2: preMods, Idx 3: team mods
		Stats::Sheet<Stats::Value<Formula::Context, 4>> postMods{};
		// Idx 0: kit, Idx 1: weapon and artifact
		Stats::Sheet<Stats::Value<Formula::Context, 2>> teamPreMods{};
		Stats::Sheet<Stats::Value<Formula::Context, 2>> teamPostMods{};
		Stats::EnemySheet<Stats::Value<Formula::Context, 2>> enemySheet{};

		Formula::ElementNode infusion;
		Formula::ElementNode teamInfusion;

		Talents talents{};
		uint8_t constellation{};
		uint8_t level{1};
		uint8_t ascension{0};

		void init(Stats::Loadout &stats);

	private:
		void linkWeaponAndArtifacts();
	};
}// namespace Stats
