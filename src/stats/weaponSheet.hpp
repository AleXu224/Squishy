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

		Stats::Sheet<Stats::Value<float, 2>> preMods{};
		Stats::Sheet<Stats::Value<float, 1>> postMods{};
		Stats::Sheet<Stats::Value<float, 1>> teamPreMods{};
		Stats::Sheet<Stats::Value<float, 1>> teamPostMods{};
		Stats::EnemySheet<Stats::Value<float, 1>> enemySheet{};

		Talents<Stats::Value<uint32_t, 1>> talents{};
		Talents<Stats::Value<uint32_t, 1>> teamTalents{};

		explicit WeaponSheet(const WeaponBase &base);
	};
}// namespace Stats
