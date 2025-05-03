#include "instance.hpp"

#include "store.hpp"
#include "weapon/weapons.hpp"


Weapon::Instance::Instance(const Weapon::DataKey &dataKey, const Weapon::InstanceKey &instanceKey)
	: instanceKey(instanceKey),
	  stats(Weapon::list.at(dataKey)) {}

void Weapon::Instance::equipOn(Character::InstanceKey instanceKey, std::optional<uint32_t> loadoutIndex) const {
	if (!instanceKey) return;

	auto &character = ::Store::characters.at(instanceKey);
	auto &loadout = character.state.loadoutByIndex(loadoutIndex);

	loadout.swapWeapon(this->instanceKey);
	stats.data->getOpts(character.state.options);
	character.updateEvent.notify();
	updateEvent.notify();
}

std::vector<Character::InstanceKey> Weapon::Instance::equippedOn() const {
	std::vector<Character::InstanceKey> ret;
	for (const auto &[key, _]: Store::characters) {
		if (isEquippedOn(key)) ret.emplace_back(key);
	}
	return ret;
}

bool Weapon::Instance::isEquippedOn(Character::InstanceKey instanceKey) const {
	if (!instanceKey) return false;

	auto &character = Store::characters.at(instanceKey);

	if (character.state.equippedLoadout.weaponInstanceKey == this->instanceKey) return true;

	return false;
}

std::vector<Character::InstanceKey> Weapon::Instance::usedOn() const {
	std::vector<Character::InstanceKey> ret;

	for (const auto &[key, _]: Store::characters) {
		if (isUsedOn(key)) ret.emplace_back(key);
	}

	return ret;
}

bool Weapon::Instance::isUsedOn(Character::InstanceKey instanceKey) const {
	if (!instanceKey) return false;

	auto &character = Store::characters.at(instanceKey);

	if (character.state.equippedLoadout.weaponInstanceKey == this->instanceKey) return true;

	for (const auto &loadout: character.state.loadouts) {
		if (loadout.weaponInstanceKey == this->instanceKey) return true;
	}

	return false;
}
