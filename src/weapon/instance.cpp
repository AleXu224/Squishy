#include "instance.hpp"

#include "store.hpp"
#include "weapon/weapons.hpp"


Weapon::Instance::Instance(const Weapon::DataKey &dataKey, const Weapon::InstanceKey &instanceKey)
	: instanceKey(instanceKey),
	  stats(Weapon::list.at(dataKey)) {}

void Weapon::Instance::equipOn(Character::InstanceKey instanceKey) {
	auto &character = ::Store::characters.at(instanceKey);

	character.loadout.weapon = &this->stats;
	character.weaponInstanceKey = this->instanceKey;
	stats.data->getOpts(character.loadout.options);
	character.updateEvent.notify();
	updateEvent.notify();
}
