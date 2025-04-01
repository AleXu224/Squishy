#include "IWeapon.hpp"

#include "character/data.hpp"
#include "store.hpp"
#include "weapon/data.hpp"
#include "weapon/weapons.hpp"

std::vector<Serialization::Good::IWeapon> Serialization::Good::IWeapon::fromInstance(const Weapon::Instance &weapon) {
	auto equippedCharacters = [&]() {
		std::vector<std::string_view> ret;

		for (const auto &[_, character]: Store::characters) {
			if (character.weaponInstanceKey == weapon.instanceKey)
				ret.emplace_back(character.loadout.character.data.goodKey);
		}

		// Add an empty string for unequipped weapons
		if (ret.empty()) ret.emplace_back("");

		return ret;
	}();

	std::vector<Serialization::Good::IWeapon> ret;

	// Weapons works differently here compared to how they are in the GOOD format
	// the GOOD format assumes a weapon can only be equipped in one place, while
	// here they can be equipped by multiple characters at once
	// Because of this we have to create multiple copies of this weapon for every
	// instance of this weapon being equipped
	for (const auto &goodKey: equippedCharacters) {
		ret.emplace_back(Serialization::Good::IWeapon{
			.key{weapon.stats.data->goodKey},
			.level = weapon.stats.sheet.level,
			.ascension = weapon.stats.sheet.ascension,
			.refinement = weapon.stats.sheet.refinement,
			.location{goodKey},
		});
	}

	return ret;
}

std::expected<std::reference_wrapper<Weapon::Instance>, std::string> Serialization::Good::IWeapon::createInstance() const {
	auto weaponData = getData();
	if (!weaponData) return std::unexpected(weaponData.error());
	const auto &data = weaponData.value();

	auto &weapon = Store::createWeapon(data.get().key);
	writeToInstance(weapon);
	return std::ref(weapon);
}

std::expected<std::reference_wrapper<const Weapon::Data>, std::string> Serialization::Good::IWeapon::getData() const {
	for (const auto &data: Weapon::list) {
		if (data.second.goodKey == key) return std::ref(data.second);
	}
	return std::unexpected(std::format("Data for \"{}\" not found", key));
}

std::expected<std::reference_wrapper<Weapon::Instance>, std::string> Serialization::Good::IWeapon::isAlreadyStored() const {
	for (auto &entry: Store::weapons) {
		if (entry.second.stats.data->goodKey == key) return std::ref(entry.second);
	}
	return std::unexpected("Weapon not found");
}

void Serialization::Good::IWeapon::writeToInstance(Weapon::Instance &weapon) const {
	weapon.stats.sheet.level = level;
	weapon.stats.sheet.ascension = ascension;
	weapon.stats.sheet.refinement = refinement;

	Character::InstanceKey equippedCharacter{};
	for (const auto &[_, character]: ::Store::characters) {
		if (character.loadout.character.data.goodKey == location) {
			equippedCharacter = character.instanceKey;
			break;
		}
	}
	if (equippedCharacter) weapon.equipOn(equippedCharacter);
}
