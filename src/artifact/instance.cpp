#include "instance.hpp"

#include "store.hpp"

void Artifact::Instance::unequip() {
	if (!equippedCharacter) return;

	auto &character = Store::characters.at(equippedCharacter);
	character.loadout.artifact.equipped.fromSlot(this->slot) = std::nullopt;

	equippedCharacter.clear();

    character.getArtifactStats();

    character.updateEvent.notify();
    this->updateEvent.notify();
}

void Artifact::Instance::equipOn(Character::InstanceKey characterKey) {
	unequip();

	if (!characterKey) return;

	auto &character = Store::characters.at(characterKey);
	character.loadout.artifact.equipped.fromSlot(this->slot) = this->key;
	this->equippedCharacter = characterKey;

    character.getArtifactStats();

    character.updateEvent.notify();
	this->updateEvent.notify();
}
