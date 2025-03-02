#include "ICharacter.hpp"

#include "store.hpp"
#include "weapon/defaultWeapons.hpp"

Serialization::Good::ICharacter Serialization::Good::ICharacter::fromInstance(const Character::Instance &character) {
	return {
		.key = std::string(Character::list.at(character.dataKey).goodKey),
		.level = character.loadout.character.sheet.level,
		.constellation = character.loadout.character.sheet.constellation,
		.ascension = character.loadout.character.sheet.ascension,
		.talent{
			.auto_ = character.loadout.character.sheet.talents.normal.constant,
			.skill = character.loadout.character.sheet.talents.skill.constant,
			.burst = character.loadout.character.sheet.talents.burst.constant,
		},
	};
}

std::expected<Character::Instance, std::string> Serialization::Good::ICharacter::toInstance() const {
	const auto &characterData = [&]() -> std::expected<std::reference_wrapper<const Character::Data>, std::string> {
		for (const auto &[_, data]: Character::list) {
			if (data.goodKey == key) return data;
		}
		return std::unexpected(std::format("Data for \"{}\" not found", key));
	}();
	if (!characterData) return std::unexpected(characterData.error());
	const auto &data = characterData.value().get();

	auto character = [&]() -> Character::Instance * {
		for (auto &[_, character]: ::Store::characters) {
			if (character.dataKey == data.key) return &character;
		}
		return nullptr;
	}();

	if (character) {
		character->loadout.character.sheet.level = level;
		character->loadout.character.sheet.constellation = constellation;
		character->loadout.character.sheet.ascension = ascension;
		character->loadout.character.sheet.talents.normal.constant = talent.auto_;
		character->loadout.character.sheet.talents.skill.constant = talent.skill;
		character->loadout.character.sheet.talents.burst.constant = talent.burst;
		return *character;
	}

	const auto &weapon = Store::weapons.emplace(Store::lastWeaponId, Weapon::Instance(Weapon::defaultWeapons.at(data.baseStats.weaponType), {++Store::lastWeaponId}));
	return Character::Instance({++Store::lastCharacterId}, data.key, weapon.first->first);
}
