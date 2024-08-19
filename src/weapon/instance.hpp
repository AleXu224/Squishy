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
			data.getConds(stats.conditionals, stats);
			Stats::setupModifiers(data.data.mods.preMod, stats.sheet.stats.preMods, 0);
			Stats::setupModifiers(data.data.mods.postMod, stats.sheet.stats.postMods, 0);
		}
	};
}// namespace Weapon
