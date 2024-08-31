#pragma once

#include "stats/sheet.hpp"
#include "weaponBase.hpp"
#include "value.hpp"


namespace Stats {
	struct Loadout;
	struct Weapon;
	struct WeaponSheet {
		unsigned short level{1};
		uint8_t ascension{0};
		unsigned short refinement{0};

		[[nodiscard]] float getBaseAtk();
		[[nodiscard]] float getSubstat();

		Stats::Sheet<Stats::Value<Formula::Context, 2>> preMods{};
		Stats::Sheet<Stats::Value<Formula::Context, 1>> postMods{};
		Stats::Sheet<Stats::Value<Formula::Context, 1>> teamPreMods{};
		Stats::Sheet<Stats::Value<Formula::Context, 1>> teamPostMods{};
		Stats::EnemySheet<Stats::Value<Formula::Context, 1>> enemySheet{};

		explicit WeaponSheet(const WeaponBase &base);
	};
}// namespace Stats
