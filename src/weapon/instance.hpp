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
			data.getMods(stats.conditionals, stats);
		}
	};
}// namespace Weapon
