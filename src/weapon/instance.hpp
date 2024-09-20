#pragma once

#include "stats/weapon.hpp"
#include "weapon/data.hpp"


namespace Weapon {
	struct Instance {
		const InstanceKey instanceKey;
		Stats::Weapon stats;
		const Weapon::Data &data;

		Instance(const Weapon::DataKey &dataKey, const Weapon::InstanceKey &instanceKey);
	};
}// namespace Weapon
