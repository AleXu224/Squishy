#pragma once

#include "Talents.hpp"
#include "stats/sheet.hpp"


namespace Stats {
	struct Loadout;
	struct CharacterSheet {
		StatSheet<3, 3, 0, 0> stats{
			.preMods{
				.er{.constant = 1.f},
				.cr{.constant = 0.05f},
				.cd{.constant = 0.5f},
			},
		};

		Talents talents{};
		uint8_t constellation{};
		uint8_t level{1};
		uint8_t ascension{0};

		void init(Stats::Loadout &sheet);

	private:
		void linkWeaponAndArtifacts(Stats::Loadout &);
	};
}// namespace Stats
