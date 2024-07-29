#pragma once

#include "stats/weapon.hpp"
#include "weapon/data.hpp"
#include "weapon/weapons.hpp"



namespace Weapon {
	struct Instance {
		Stats::Weapon stats;
		const Weapon::Data &data;

		// void collectStats(Stats::Character &characterStats) const {
		// 	characterStats.sheet.addModifier(Stat::baseAtk, [](const Stats::CharacterSheet &sheet) -> float {
		// 		return sheet.getStat(Stat::atk);
		// 	});
		// 	characterStats.sheet.addModifier(stats.base.substat.stat, [&](const Stats::CharacterSheet &) {
		// 		return this->stats.sheet.subStat.calculateValue(this->stats.sheet);
		// 	});
		// }

		explicit Instance(const Weapon::Key &key)
			: stats(Weapon::list.at(key).baseStats), data(Weapon::list.at(key)) {}
	};
}// namespace Weapon
