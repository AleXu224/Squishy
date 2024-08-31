#pragma once

#include "stats/weapon.hpp"
#include "weapon/data.hpp"
#include "weapon/weapons.hpp"


namespace Weapon {
	struct Instance {
		Stats::Weapon stats;
		const Weapon::Data &data;

		explicit Instance(const Weapon::Key &key)
			: stats(Weapon::list.at(key)), data(Weapon::list.at(key)) {
			data.getConds(stats.conditionals);
			Stats::setupModifiers(data.data.mods.preMod, stats.sheet.preMods, 0);
			Stats::setupModifiers(data.data.mods.postMod, stats.sheet.postMods, 0);
			Stats::setupModifiers(data.data.mods.teamPreMod, stats.sheet.teamPreMods, 0);
			Stats::setupModifiers(data.data.mods.teamPostMod, stats.sheet.teamPostMods, 0);
			Stats::setupModifiers(data.data.mods.enemy, stats.sheet.enemySheet, 0);
		}
	};
}// namespace Weapon
