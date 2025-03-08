#include "store.hpp"

#include "fstream"
#include "weapon/defaultWeapons.hpp"

#include "glaze/glaze.hpp"           // IWYU pragma: keep
#include "serialization/save/key.hpp"// IWYU pragma: keep
#include "serialization/save/save.hpp"

Character::Instance &Store::createCharacter(Character::DataKey dataKey, Weapon::InstanceKey weapon) {
	auto &data = Character::list.at(dataKey);

	if (!weapon) {
		weapon = createWeapon(Weapon::defaultWeapons.at(data.baseStats.weaponType)).instanceKey;
	}

	++lastCharacterId;
	auto entry = characters.emplace(lastCharacterId, Character::Instance(lastCharacterId, dataKey, weapon));

	return entry.first->second;
}

Weapon::Instance &Store::createWeapon(Weapon::DataKey dataKey) {
	++lastWeaponId;
	auto entry = weapons.emplace(lastWeaponId, Weapon::Instance(dataKey, lastWeaponId));

	return entry.first->second;
}

Artifact::Instance &Store::createArtifact(Artifact::SetKey setKey) {
	++lastArtifactId;
	auto entry = artifacts.emplace(lastArtifactId, Artifact::Instance{.key = lastArtifactId, .set = setKey});

	return entry.first->second;
}

Team::Instance &Store::createTeam(std::string_view name) {
	++lastTeamId;
	auto entry = teams.emplace(lastTeamId, Team::Instance{.instanceKey = lastTeamId});

	return entry.first->second;
}

namespace {
	Serialization::Save::Save save() {
		std::vector<Serialization::Save::Artifact> retArtifacts{};
		retArtifacts.reserve(::Store::artifacts.size());
		for (const auto &[_, artifactInstance]: ::Store::artifacts) {
			retArtifacts.emplace_back(Serialization::Save::Artifact::fromInstance(artifactInstance));
		}

		std::vector<Serialization::Save::Weapon> retWeapons{};
		retWeapons.reserve(::Store::weapons.size());
		for (const auto &[_, weaponInstance]: ::Store::weapons) {
			retWeapons.emplace_back(Serialization::Save::Weapon::fromInstance(weaponInstance));
		}

		std::vector<Serialization::Save::Character> retCharacters{};
		retCharacters.reserve(::Store::characters.size());
		for (const auto &[_, characterInstance]: ::Store::characters) {
			retCharacters.emplace_back(Serialization::Save::Character::fromInstance(characterInstance));
		}

		std::vector<Serialization::Save::Team> retTeams{};
		retTeams.reserve(::Store::teams.size());
		for (const auto &[_, teamInstance]: ::Store::teams) {
			retTeams.emplace_back(Serialization::Save::Team::fromInstance(teamInstance));
		}

		return Serialization::Save::Save{
			.artifacts = retArtifacts,
			.weapons = retWeapons,
			.characters = retCharacters,
			.teams = retTeams,
		};
	}

	void load(const Serialization::Save::Save &save) {
		::Store::artifacts.clear();
		::Store::weapons.clear();
		::Store::characters.clear();
		::Store::teams.clear();
		::Store::enemies.clear();

		uint32_t maxArtifactKey = 1;
		for (const auto &artifact: save.artifacts) {
			maxArtifactKey = std::max(maxArtifactKey, artifact.instanceKey.key);
			::Store::artifacts.emplace(artifact.instanceKey, artifact.toInstance());
		}

		uint32_t maxWeaponKey = 1;
		for (const auto &weapon: save.weapons) {
			maxWeaponKey = std::max(maxWeaponKey, weapon.instanceKey.key);
			::Store::weapons.emplace(weapon.instanceKey, weapon.toInstance());
		}

		uint32_t maxCharacterKey = 1;
		for (const auto &character: save.characters) {
			maxCharacterKey = std::max(maxCharacterKey, character.instanceKey.key);
			::Store::characters.emplace(character.instanceKey, character.toInstance());
		}

		uint32_t maxTeamKey = 1;
		for (const auto &team: save.teams) {
			maxTeamKey = std::max(maxTeamKey, team.instanceKey.key);
			::Store::teams.emplace(team.instanceKey, team.toInstance());
		}

		::Store::lastArtifactId = maxArtifactKey;
		::Store::lastWeaponId = maxWeaponKey;
		::Store::lastCharacterId = maxCharacterKey;
		::Store::lastTeamId = maxTeamKey;
	}
}// namespace

void Store::saveToFile(const std::string &strpath) {
	std::filesystem::path path = strpath;
	if (!path.has_filename()) throw std::runtime_error("Path must point to a file");
	std::filesystem::create_directory(path.parent_path());
	auto file = std::ofstream(path, std::ofstream::binary);
	{
		// Using json at the moment for debugability
		std::string buff{};
		auto d = glz::write<glz::opts{.prettify = true}>(save(), buff);
		if (d) {
			std::println("Failed saving");
		}
		file << buff;
	}
	file.close();
}

void Store::loadFromFile(const std::string &strpath) {
	std::filesystem::path path = strpath;
	if (!path.has_filename()) throw std::runtime_error("Path must point to a file");
	auto file = std::ifstream(path, std::ofstream::binary);
	if (!file.is_open()) return;

	std::stringstream ss{};
	ss << file.rdbuf();
	Serialization::Save::Save saveData;
	auto d = glz::read<glz::opts{.error_on_unknown_keys = false}>(saveData, ss.str());
	if (d) {
		std::println("Failed loading save file");
		return;
	}
	file.close();

	load(saveData);
}

extern Serialization::Good::IGOOD Store::saveToGOOD() {
	Serialization::Good::IGOOD ret{};

	std::vector<Serialization::Good::IArtifact> artifacts;
	for (const auto &[_, artifact]: ::Store::artifacts) {
		artifacts.emplace_back(Serialization::Good::IArtifact::fromInstance(artifact));
	}

	std::vector<Serialization::Good::IWeapon> weapons;
	for (const auto &[_, weapon]: ::Store::weapons) {
		auto weps = Serialization::Good::IWeapon::fromInstance(weapon);
		weapons.insert(weapons.end(), weps.begin(), weps.end());
	}

	std::vector<Serialization::Good::ICharacter> characters;
	for (const auto &[_, character]: ::Store::characters) {
		characters.emplace_back(Serialization::Good::ICharacter::fromInstance(character));
	}

	ret.artifacts = artifacts;
	ret.weapons = weapons;
	ret.characters = characters;

	return ret;
}

extern void Store::loadFromGOOD(const Serialization::Good::IGOOD &data) {
	if (data.characters.has_value()) {
		for (const auto &character: data.characters.value()) {
			auto storedCharacter = character.isAlreadyStored();
			if (!storedCharacter) storedCharacter = character.createInstance();
			if (!storedCharacter) {
				std::println("{}", storedCharacter.error());
				continue;
			}

			character.writeToInstance(storedCharacter.value());
		}
	}
	if (data.weapons.has_value()) {
		for (const auto &weapon: data.weapons.value()) {
			auto storedWeapon = weapon.isAlreadyStored();
			if (!storedWeapon) storedWeapon = weapon.createInstance();
			if (!storedWeapon) {
				std::println("{}", storedWeapon.error());
				continue;
			}

			weapon.writeToInstance(storedWeapon.value());
		}
	}
	if (data.artifacts.has_value()) {
		for (const auto &artifact: data.artifacts.value()) {
			if (artifact.setKey == "ScrollOfTheHeroOfCinderCity") {
				(void) 1;
			}
				auto storedArtifact = artifact.isAlreadyStored();
			if (!storedArtifact && !(storedArtifact = artifact.createInstance())) {
				std::println("{}", storedArtifact.error());
				continue;
			}

			artifact.writeToInstance(storedArtifact.value());
		}
	}
}
