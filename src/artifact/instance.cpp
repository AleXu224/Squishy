#include "instance.hpp"

#include "store.hpp"

void Artifact::Instance::unequip() {
	if (!equippedCharacter) return;

	auto &character = Store::characters.at(equippedCharacter);
	character.loadout.artifact.equipped.fromSlot(this->slot) = std::nullopt;

	equippedCharacter.clear();

	character.loadout.artifact.refreshStats();

	character.updateEvent.notify();
	this->updateEvent.notify();
}

void Artifact::Instance::equipOn(Character::InstanceKey characterKey) {
	if (!characterKey) return;

	auto &character = Store::characters.at(characterKey);
	auto &slot = character.loadout.artifact.equipped.fromSlot(this->slot);
	if (slot) {
		auto &artifact = Store::artifacts.at(slot.value());
		if (this->equippedCharacter) {
			auto otherCharacter = this->equippedCharacter;
			unequip();
			artifact.equipOn(otherCharacter);
		} else {
			artifact.unequip();
		}
	}
	slot = this->key;
	this->equippedCharacter = characterKey;

	character.loadout.artifact.refreshStats();

	character.updateEvent.notify();
	this->updateEvent.notify();
}
