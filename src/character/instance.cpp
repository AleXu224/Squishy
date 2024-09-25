#include "instance.hpp"
#include "artifact/instance.hpp"
#include "artifact/sets.hpp"
#include "character/characters.hpp"
#include "formula/teamCharacter.hpp"
#include "store.hpp"


Character::Instance::Instance(const InstanceKey &instanceKey, const DataKey &dataKey, const Weapon::InstanceKey &weaponInstanceKey)
	: instanceKey(instanceKey),
	  dataKey(dataKey),
	  weaponInstanceKey(weaponInstanceKey),
	  loadout(Stats::Loadout{
		  .character = Stats::Character(Character::list.at(dataKey)),
		  .weapon = Store::weapons.at(weaponInstanceKey).stats,
		  .artifact{},
	  }) {
	const auto &character = Character::list.at(dataKey);
	character.getOpts(loadout.character.options);

	Stats::setupModifiers(character.data.mods.preMod, loadout.character.sheet.preMods, 0);
	Stats::setupModifiers(character.data.mods.postMod, loadout.character.sheet.postMods, 0);
	Stats::setupModifiers(character.data.mods.teamPreMod, loadout.character.sheet.teamPreMods, 0);
	Stats::setupModifiers(character.data.mods.teamPostMod, loadout.character.sheet.teamPostMods, 0);
	Stats::setupModifiers(character.data.mods.enemy, loadout.character.sheet.enemySheet, 0);

	loadout.character.sheet.infusion = Formula::CharacterTeamInfusion(character.data.mods.infusion);
	loadout.character.sheet.teamInfusion = character.data.mods.teamInfusion;

	Stats::setupTalents(character.data.mods.talents, loadout.character.sheet.talents, 0);
	Stats::setupTalents(character.data.mods.teamTalents, loadout.character.sheet.teamTalents, 0);

	loadout.character.sheet.init(loadout);

	for (const auto &[setKey, set]: Artifact::sets) {
		set.getOptions(loadout.artifact.options);
	}

	loadout.artifact.sheet.init();
}

void Character::Instance::getArtifactStats() {
	static std::unordered_map<Artifact::SetKey, uint8_t> occurences{};
	loadout.artifact.bonus1 = std::nullopt;
	loadout.artifact.bonus2 = std::nullopt;
	for (auto &artifact: loadout.artifact.sheet.equippedArtifacts) {
		artifact = std::nullopt;
	}

	for (const auto &[artPtr, index]: std::views::zip(Stats::Artifact::Slotted::getMembers(), std::views::iota(0, 5))) {
		auto artId = std::invoke(artPtr, loadout.artifact.equipped);
		if (!artId.has_value()) continue;
		auto &artifact = Store::artifacts.at(artId.value());
		occurences[artifact.set]++;
		loadout.artifact.sheet.equippedArtifacts.at(index) = std::make_optional(&artifact.stats);
	}
	for (auto &occurence: occurences) {
		if (occurence.second >= 2) {
			// Cache the ArtifactData to avoid doing two accesses
			const Artifact::Set &artifactData = Artifact::sets.at(occurence.first);

			auto &targetBonus = [&]() -> std::optional<Stats::ArtifactBonus> & {
				if (!loadout.artifact.bonus1.has_value()) return loadout.artifact.bonus1;

				return loadout.artifact.bonus2;
			}();
			targetBonus.emplace(Stats::ArtifactBonus{
				.setPtr = artifactData,
				.bonusPtr = artifactData.data.twoPc,
			});

			// The second check should not be be outside since a four set can only happen
			// only if there is a two set
			if (occurence.second >= 4) {
				loadout.artifact.bonus2.emplace(Stats::ArtifactBonus{
					.setPtr = artifactData,
					.bonusPtr = artifactData.data.fourPc,
				});
			}
		}
	}
	occurences.clear();
}
