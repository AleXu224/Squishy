#pragma once

#include "stats/weapon.hpp"
#include "weapon/data.hpp"
#include "weapon/weapons.hpp"


namespace Weapon {
	struct Instance {
		const InstanceKey instanceKey;
		Stats::Weapon stats;
		const Weapon::Data &data;

		explicit Instance(const Weapon::DataKey &dataKey, const Weapon::InstanceKey &instanceKey)
			: instanceKey(instanceKey),
			  stats(Weapon::list.at(dataKey)),
			  data(Weapon::list.at(dataKey)) {
			data.getOpts(stats.options);
			Stats::setupModifiers(data.data.mods.preMod, stats.sheet.preMods, 0);
			Stats::setupModifiers(data.data.mods.postMod, stats.sheet.postMods, 0);
			Stats::setupModifiers(data.data.mods.teamPreMod, stats.sheet.teamPreMods, 0);
			Stats::setupModifiers(data.data.mods.teamPostMod, stats.sheet.teamPostMods, 0);
			Stats::setupModifiers(data.data.mods.enemy, stats.sheet.enemySheet, 0);
			Stats::setupTalents(data.data.mods.talents, stats.sheet.talents, 0);
			Stats::setupTalents(data.data.mods.teamTalents, stats.sheet.teamTalents, 0);
		}
	};
}// namespace Weapon
