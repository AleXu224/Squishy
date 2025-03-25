#pragma once

#include "observer.hpp"

#include "character/key.hpp"
#include "combo/combo.hpp"
#include "stats/loadout.hpp"
#include "weapon/key.hpp"


namespace Character {
	struct Instance {
		const InstanceKey instanceKey;
		const DataKey dataKey;
		Weapon::InstanceKey weaponInstanceKey;
		Stats::Loadout loadout;
		std::unordered_map<Combo::InstanceKey, Combo::Combo> combos;
		squi::VoidObservable updateEvent{};

		explicit Instance(const InstanceKey &instanceKey, const DataKey &dataKey, const Weapon::InstanceKey &weaponInstanceKey);
	};
}// namespace Character
