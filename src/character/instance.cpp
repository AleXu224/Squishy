#include "instance.hpp"

#include "character/characters.hpp"
#include "store.hpp"

Character::Instance::Instance(const InstanceKey &instanceKey, const DataKey &dataKey, const Weapon::InstanceKey &weaponInstanceKey)
	: instanceKey(instanceKey),
	  dataKey(dataKey),
	  weaponInstanceKey(weaponInstanceKey),
	  loadout(Stats::Loadout{
		  .character = Stats::Character(Character::list.at(dataKey)),
		  .weapon = &Store::weapons.at(weaponInstanceKey).stats,
		  .artifact{},
	  }) {
	loadout.init();
}
