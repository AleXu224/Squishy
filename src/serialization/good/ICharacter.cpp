#include "ICharacter.hpp"

#include "character/characters.hpp"
#include "store.hpp"

Serialization::Good::ICharacter Serialization::Good::ICharacter::fromInstance(const Character::Instance &character) {
	return {
		.key = std::string(Character::list.at(character.dataKey).goodKey),
		.level = character.state.stats.sheet.level,
		.constellation = character.state.stats.sheet.constellation,
		.ascension = character.state.stats.sheet.ascension,
		.talent{
			.auto_ = character.state.stats.sheet.talents.normal.constant + 1,
			.skill = character.state.stats.sheet.talents.skill.constant + 1,
			.burst = character.state.stats.sheet.talents.burst.constant + 1,
		},
	};
}

std::expected<std::reference_wrapper<Character::Instance>, std::string> Serialization::Good::ICharacter::createInstance() const {
	auto characterData = getData();
	if (!characterData) return std::unexpected(characterData.error());
	const auto &data = characterData.value().get();

	auto &instance = Store::createCharacter(data.key);
	writeToInstance(instance);

	return instance;
}

std::expected<std::reference_wrapper<const Character::Data>, std::string> Serialization::Good::ICharacter::getData() const {
	for (const auto &[_, data]: Character::list) {
		if (data.goodKey == key) return data;
	}
	return std::unexpected(std::format("Data for \"{}\" not found", key));
}

std::expected<std::reference_wrapper<Character::Instance>, std::string> Serialization::Good::ICharacter::isAlreadyStored() const {
	auto characterData = getData();
	if (!characterData) return std::unexpected(characterData.error());
	auto &data = characterData.value().get();
	for (auto &[_, character]: ::Store::characters) {
		if (character.dataKey == data.key) return std::ref(character);
	}
	return std::unexpected("Character not found");
}

void Serialization::Good::ICharacter::writeToInstance(Character::Instance &character) const {
	character.state.stats.sheet.level = level;
	character.state.stats.sheet.constellation = constellation;
	character.state.stats.sheet.ascension = ascension;
	character.state.stats.sheet.talents.normal.constant = talent.auto_ - 1;
	character.state.stats.sheet.talents.skill.constant = talent.skill - 1;
	character.state.stats.sheet.talents.burst.constant = talent.burst - 1;
}
