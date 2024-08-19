#pragma once

#include "stats/sheet.hpp"
#include "weaponBase.hpp"


namespace Stats {
	struct Loadout;
	struct Weapon;
	struct WeaponSheet {
		unsigned short level{1};
		uint8_t ascension{0};
		unsigned short refinement{0};

		[[nodiscard]] float getBaseAtk();
		[[nodiscard]] float getSubstat();

		StatSheet<2, 1, 0, 0> stats{};

		explicit WeaponSheet(const WeaponBase &base);
	};
}// namespace Stats
