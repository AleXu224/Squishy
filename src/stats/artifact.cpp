#include "artifact.hpp"

#include "store.hpp"

void Stats::Artifact::Slotted::unequipAll() {
	for (const auto &member: getMembers()) {
		auto &slot = std::invoke(member, *this);

		if (!slot.has_value()) continue;
		auto &artifact = ::Store::artifacts.at(slot.value());
		artifact.equippedCharacter.clear();
	}
}
