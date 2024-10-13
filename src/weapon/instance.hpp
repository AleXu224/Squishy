#pragma once

#include "observer.hpp"
#include "stats/weapon.hpp"
#include "weapon/key.hpp"

namespace Weapon {
	struct Instance {
		InstanceKey instanceKey;
		Stats::Weapon stats;

		squi::VoidObservable updateEvent{};

		Instance(const Weapon::DataKey &dataKey, const Weapon::InstanceKey &instanceKey);
	};
}// namespace Weapon
