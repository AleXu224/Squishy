#include "instance.hpp"

#include "store.hpp"

void Artifact::Instance::unequip() const {
	for (auto &[_, character]: Store::characters) {
		auto &equippedKey = character.state.equippedLoadout.artifact.getSlotted().fromSlot(this->slot);
		if (equippedKey == key) {
			equippedKey.clear();
			character.state.equippedLoadout.artifact.refreshStats();
			character.updateEvent.notify();
		}
		for (auto &loadout: character.state.loadouts) {
			std::visit(
				Utils::overloaded{
					[&](Stats::Artifact::Slotted &slotted) {
						auto &equippedKey = slotted.fromSlot(slot);
						if (equippedKey == key) {
							equippedKey.clear();
							loadout.artifact.refreshStats();
							character.updateEvent.notify();
						}
					},
					[](Stats::Artifact::Theorycraft &theorycraft) {},
				},
				loadout.artifact.equipped
			);
		}
	}
	this->updateEvent.notify();
}

void Artifact::Instance::equipOn(Character::InstanceKey characterKey, std::optional<uint32_t> loadoutIndex) const {
	if (!characterKey) return;

	auto &character = Store::characters.at(characterKey);
	auto &loadout = character.state.loadoutByIndex(loadoutIndex);

	if (!std::holds_alternative<Stats::Artifact::Slotted>(loadout.artifact.equipped)) return;
	auto &slotted = loadout.artifact.getSlotted();

	auto &equippedKey = slotted.fromSlot(this->slot);

	if (!loadoutIndex && equippedKey != this->key) {
		auto alreadyEquippedLocation = equippedOn();
		if (equippedKey && alreadyEquippedLocation) {
			auto &otherCharacter = Store::characters.at(alreadyEquippedLocation);
			otherCharacter.state.equippedLoadout.artifact.getSlotted().fromSlot(this->slot) = equippedKey;
			Store::artifacts.at(equippedKey).updateEvent.notify();
			otherCharacter.updateEvent.notify();
		} else if (alreadyEquippedLocation) {
			auto &otherCharacter = Store::characters.at(alreadyEquippedLocation);
			otherCharacter.state.equippedLoadout.artifact.getSlotted().fromSlot(this->slot).clear();
			otherCharacter.updateEvent.notify();
		}
	}

	auto equippedKeyCopy = equippedKey;

	equippedKey = this->key;

	if (equippedKeyCopy) Store::artifacts.at(equippedKeyCopy).updateEvent.notify();

	loadout.artifact.refreshStats();
	character.updateEvent.notify();
	updateEvent.notify();
}

Character::InstanceKey Artifact::Instance::equippedOn() const {
	Character::InstanceKey ret{};
	for (auto &[characterKey, character]: Store::characters) {
		auto &equippedKey = character.state.equippedLoadout.artifact.getSlotted().fromSlot(this->slot);
		if (equippedKey == key) {
			ret = characterKey;
			break;
		}
	}
	return ret;
}

std::vector<Character::InstanceKey> Artifact::Instance::usedOn() const {
	std::vector<Character::InstanceKey> ret{};
	for (auto &[characterKey, character]: Store::characters) {
		auto &equippedKey = character.state.equippedLoadout.artifact.getSlotted().fromSlot(this->slot);
		if (equippedKey == key) {
			ret.emplace_back(characterKey);
			continue;
		}
		for (const auto &loadout: character.state.loadouts) {
			if (!std::holds_alternative<Stats::Artifact::Slotted>(loadout.artifact.equipped)) continue;
			const auto &artifactKey = loadout.artifact.getSlotted().fromSlot(this->slot);
			if (artifactKey == this->key) {
				ret.emplace_back(characterKey);
				continue;
			}
		}
	}
	return ret;
}

void Artifact::Instance::refreshUsages() const {
	for (auto &[characterKey, character]: Store::characters) {
		auto &equippedKey = character.state.equippedLoadout.artifact.getSlotted().fromSlot(this->slot);
		if (equippedKey == key) {
			character.state.equippedLoadout.artifact.refreshStats();
			character.updateEvent.notify();
		}
		for (auto &loadout: character.state.loadouts) {
			if (!std::holds_alternative<Stats::Artifact::Slotted>(loadout.artifact.equipped)) continue;
			const auto &artifactKey = loadout.artifact.getSlotted().fromSlot(this->slot);
			if (artifactKey == this->key) {
				loadout.artifact.refreshStats();
				character.updateEvent.notify();
			}
		}
	}
}
