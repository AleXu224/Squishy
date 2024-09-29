#pragma once

#include "stats/sheet.hpp"
#include "weaponBase.hpp"
#include "value.hpp"


namespace Stats {
	struct Loadout;
	struct Weapon;
	struct WeaponSheet {
		uint8_t level{1};
		uint8_t ascension{0};
		uint8_t refinement{0};

		[[nodiscard]] float getBaseAtk();
		[[nodiscard]] float getSubstat();

		Stats::Sheet<Stats::Value<float, 1>> stats{};

		explicit WeaponSheet(const WeaponBase &base);
	};
}// namespace Stats
