#include "store.hpp"

#include "cereal/archives/json.hpp"
#include "fstream"
#include "ranges"
#include "weapon/data.hpp"

auto Store::serializeOptions(auto &&options) {
	std::vector<Serialization::Save::OptionTypes> ret{};
	for (const auto &[key, option]: options) {
		std::visit(
			Utils::overloaded{
				[&](const Option::Boolean &opt) {
					ret.emplace_back(Serialization::Save::BooleanOption{
						.hash = key,
						.active = opt.active,
					});
				},
				[&](const Option::ValueList &opt) {
					ret.emplace_back(Serialization::Save::ValueListOption{
						.hash = key,
						.index = opt.currentIndex,
					});
				},
			},
			option
		);
	}
	return ret;
}

void Store::deserializeOptions(const std::vector<Serialization::Save::OptionTypes> &options, auto &&targetMap) {
	for (const auto &option: options) {
		std::visit(
			Utils::overloaded{
				[&](const Serialization::Save::BooleanOption &opt) {
					std::visit(
						Utils::overloaded{
							[&](Option::Boolean &val) {
								val.active = opt.active;
							},
							[](const Option::ValueList &val) {
								std::println("Wrong opt selected while loading save {}", val.key.str);
							},
						},
						targetMap.at(opt.hash)
					);
				},
				[&](const Serialization::Save::ValueListOption &opt) {
					std::visit(
						Utils::overloaded{
							[](const Option::Boolean &val) {
								std::println("Wrong opt selected while loading save {}", val.key.str);
							},
							[&](Option::ValueList &val) {
								val.currentIndex = opt.index;
							},
						},
						targetMap.at(opt.hash)
					);
				},
			},
			option
		);
	}
}

Serialization::Save::Save Store::save() {
	std::vector<Serialization::Save::Artifact> retArtifacts{};
	retArtifacts.reserve(::Store::artifacts.size());
	for (const auto &[artifactKey, artifactInstance]: ::Store::artifacts) {
		retArtifacts.emplace_back(Serialization::Save::Artifact{
			.instanceKey = artifactKey,
			.setKey = artifactInstance.set,
			.slot = artifactInstance.slot,
			.mainStat = artifactInstance.mainStat,
			.subStats = [&]() {
				std::array<std::optional<Serialization::Save::ArtifactSubStat>, 4> ret{};
				for (auto [retOpt, artOpt]: std::views::zip(ret, artifactInstance.subStats)) {
					if (!artOpt.has_value()) continue;
					retOpt = Serialization::Save::ArtifactSubStat{
						.stat = artOpt->stat,
						.value = artOpt->value,
					};
				}
				return ret;
			}(),
			.level = artifactInstance.level,
			.rarity = artifactInstance.rarity,
			.equippedCharacter = artifactInstance.equippedCharacter,
		});
	}

	std::vector<Serialization::Save::Weapon> retWeapons{};
	retWeapons.reserve(::Store::weapons.size());
	for (const auto &[weaponKey, weaponInstance]: ::Store::weapons) {
		retWeapons.emplace_back(Serialization::Save::Weapon{
			.instanceKey = weaponKey,
			.dataKey = weaponInstance.stats.data->key,
			.level = weaponInstance.stats.sheet.level,
			.ascension = weaponInstance.stats.sheet.ascension,
			.refinement = weaponInstance.stats.sheet.refinement,
			.options = serializeOptions(weaponInstance.stats.options),
		});
	}

	std::vector<Serialization::Save::Character> retCharacters{};
	retCharacters.reserve(::Store::characters.size());
	for (const auto &[characterKey, characterInstance]: ::Store::characters) {
		retCharacters.emplace_back(Serialization::Save::Character{
			.instanceKey = characterKey,
			.dataKey = characterInstance.dataKey,
			.weaponInstanceKey = characterInstance.weaponInstanceKey,
			.constellation = characterInstance.loadout.character.sheet.constellation,
			.level = characterInstance.loadout.character.sheet.level,
			.ascension = characterInstance.loadout.character.sheet.ascension,
			.normalLevel = characterInstance.loadout.character.sheet.talents.normal.constant,
			.skillLevel = characterInstance.loadout.character.sheet.talents.skill.constant,
			.burstLevel = characterInstance.loadout.character.sheet.talents.burst.constant,
			.artifactFlower = characterInstance.loadout.artifact.equipped.flower,
			.artifactPlume = characterInstance.loadout.artifact.equipped.plume,
			.artifactSands = characterInstance.loadout.artifact.equipped.sands,
			.artifactGoblet = characterInstance.loadout.artifact.equipped.goblet,
			.artifactCirclet = characterInstance.loadout.artifact.equipped.circlet,
			// std::vector<Serialization::Save::OptionTypes> options;
			.options = serializeOptions(characterInstance.loadout.character.options),
			// std::vector<ArtifactOptions> artifactOptions;
			.artifactOptions = serializeOptions(characterInstance.loadout.artifact.options),
		});
	}

	std::vector<Serialization::Save::Team> retTeams{};
	retTeams.reserve(::Store::teams.size());
	for (const auto &[teamKey, teamInstance]: ::Store::teams) {
		retTeams.emplace_back(Serialization::Save::Team{
			.instanceKey = teamInstance.instanceKey,
			.name = teamInstance.name,
			.characters = [&]() {
				std::array<std::optional<::Character::InstanceKey>, 4> ret{};
				for (auto [retCharacter, storeCharacter]: std::views::zip(ret, teamInstance.stats.characters)) {
					if (!storeCharacter) continue;
					retCharacter = storeCharacter->instanceKey;
				}
				return ret;
			}(),
		});
	}


	return Serialization::Save::Save{
		.artifacts = retArtifacts,
		.weapons = retWeapons,
		.characters = retCharacters,
		.teams = retTeams,
	};
}

void Store::load(const Serialization::Save::Save &save) {
	::Store::artifacts.clear();
	::Store::weapons.clear();
	::Store::characters.clear();
	::Store::teams.clear();
	::Store::enemies.clear();

	uint32_t maxArtifactKey = 1;
	for (const auto &artifact: save.artifacts) {
		maxArtifactKey = std::max(maxArtifactKey, artifact.instanceKey.key);
		auto &artInstance = ::Store::artifacts.insert({
														  artifact.instanceKey,
														  ::Artifact::Instance{
															  .key = artifact.instanceKey,
															  .set = artifact.setKey,
															  .slot = artifact.slot,
															  .mainStat = artifact.mainStat,
															  .subStats = [&]() {
																  std::array<std::optional<StatValue>, 4> ret;
																  for (auto [retOpt, saveOpt]: std::views::zip(ret, artifact.subStats)) {
																	  if (!saveOpt.has_value()) continue;
																	  retOpt = StatValue{
																		  .stat = saveOpt->stat,
																		  .value = saveOpt->value,
																	  };
																  }
																  return ret;
															  }(),
															  .level = artifact.level,
															  .rarity = artifact.rarity,
															  .equippedCharacter = artifact.equippedCharacter,
														  },
													  })
								.first->second;
		artInstance.updateStats();
	}

	uint32_t maxWeaponKey = 1;
	for (const auto &weapon: save.weapons) {
		maxWeaponKey = std::max(maxWeaponKey, weapon.instanceKey.key);
		auto &wepInstance = ::Store::weapons.insert({weapon.instanceKey, ::Weapon::Instance(weapon.dataKey, weapon.instanceKey)}).first->second;
		wepInstance.stats.sheet.level = weapon.level;
		wepInstance.stats.sheet.ascension = weapon.ascension;
		wepInstance.stats.sheet.refinement = weapon.refinement;
		deserializeOptions(weapon.options, wepInstance.stats.options);
	}

	uint32_t maxCharacterKey = 1;
	for (const auto &character: save.characters) {
		maxCharacterKey = std::max(maxCharacterKey, character.instanceKey.key);
		auto &charInstance = ::Store::characters.insert({character.instanceKey, ::Character::Instance(character.instanceKey, character.dataKey, character.weaponInstanceKey)}).first->second;
		charInstance.loadout.character.sheet.constellation = character.constellation;
		charInstance.loadout.character.sheet.level = character.level;
		charInstance.loadout.character.sheet.ascension = character.ascension;
		charInstance.loadout.character.sheet.talents.normal.constant = character.normalLevel;
		charInstance.loadout.character.sheet.talents.skill.constant = character.skillLevel;
		charInstance.loadout.character.sheet.talents.burst.constant = character.burstLevel;
		charInstance.loadout.artifact.equipped.flower = character.artifactFlower;
		charInstance.loadout.artifact.equipped.plume = character.artifactPlume;
		charInstance.loadout.artifact.equipped.sands = character.artifactSands;
		charInstance.loadout.artifact.equipped.goblet = character.artifactGoblet;
		charInstance.loadout.artifact.equipped.circlet = character.artifactCirclet;
		deserializeOptions(character.options, charInstance.loadout.character.options);
		deserializeOptions(character.artifactOptions, charInstance.loadout.artifact.options);

		charInstance.getArtifactStats();
	}

	uint32_t maxTeamKey = 1;
	for (const auto &team: save.teams) {
		maxTeamKey = std::max(maxTeamKey, team.instanceKey.key);
		auto &teamInstance = ::Store::teams.insert({team.instanceKey, ::Team::Instance{.instanceKey = team.instanceKey, .name = team.name}}).first->second;
		teamInstance.stats.characters = [&]() {
			std::array<::Character::Instance *, 4> ret{};
			for (auto [retCharRef, saveChar]: std::views::zip(ret, team.characters)) {
				if (!saveChar.has_value()) continue;
				retCharRef = &::Store::characters.at(saveChar.value());
			}
			return ret;
		}();
	}

	::Store::lastArtifactId = maxArtifactKey;
	::Store::lastWeaponId = maxWeaponKey;
	::Store::lastCharacterId = maxCharacterKey;
	::Store::lastTeamId = maxTeamKey;
}

void Store::saveToFile(const std::filesystem::path &path) {
	if (!path.has_filename()) throw std::runtime_error("Path must point to a file");
	std::filesystem::create_directory(path.parent_path());
	auto file = std::ofstream(path, std::ofstream::binary);
	{
		// Using json at the moment for debugability
		cereal::JSONOutputArchive out(file);
		out(::Store::save());
	}
	file.close();
}

void Store::loadFromFile(const std::filesystem::path &path) {
	if (!path.has_filename()) throw std::runtime_error("Path must point to a file");
	auto file = std::ifstream(path, std::ofstream::binary);
	if (!file.is_open()) return;
	::Serialization::Save::Save saveData{};
	try {
		cereal::JSONInputArchive in(file);
		in(saveData);
	} catch (const std::runtime_error &err) {
		std::println("Failed loading save file: {}", err.what());
		return;
	}
	file.close();

	::Store::load(saveData);
}
