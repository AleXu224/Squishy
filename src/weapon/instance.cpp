#include "instance.hpp"

#include "weapon/weapons.hpp"

Weapon::Instance::Instance(const Weapon::DataKey &dataKey, const Weapon::InstanceKey &instanceKey)
	: instanceKey(instanceKey),
	  stats(Weapon::list.at(dataKey)) {}
