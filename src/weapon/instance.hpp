#pragma once

#include "character/key.hpp"
#include "observer.hpp"
#include "stats/weapon.hpp"
#include "weapon/key.hpp"


namespace Weapon {
	struct Instance {
		InstanceKey instanceKey;
		Stats::Weapon stats;

		squi::VoidObservable updateEvent{};

		Instance(const Weapon::DataKey &dataKey, const Weapon::InstanceKey &instanceKey);
		void equipOn(Character::InstanceKey instanceKey);
	};
}// namespace Weapon
