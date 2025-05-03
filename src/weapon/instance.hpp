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
		void equipOn(Character::InstanceKey instanceKey, std::optional<uint32_t> loadout = std::nullopt) const;
		[[nodiscard]] std::vector<Character::InstanceKey> equippedOn() const;
		[[nodiscard]] bool isEquippedOn(Character::InstanceKey instanceKey) const;
		[[nodiscard]] std::vector<Character::InstanceKey> usedOn() const;
		[[nodiscard]] bool isUsedOn(Character::InstanceKey instanceKey) const;
	};
}// namespace Weapon
