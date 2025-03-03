#include "IWeapon.hpp"

#include "store.hpp"

std::vector<Serialization::Good::IWeapon> Serialization::Good::IWeapon::fromInstance(const Weapon::Instance &weapon) {
	auto equippedCharacters = [&]() {
		std::vector<std::string_view> ret;

		for (const auto &[_, character]: Store::characters) {
			if (character.weaponInstanceKey == weapon.instanceKey)
				ret.emplace_back(character.loadout.character.data.goodKey);
		}

		return ret;
	}();

	std::vector<Serialization::Good::IWeapon> ret;

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
}
