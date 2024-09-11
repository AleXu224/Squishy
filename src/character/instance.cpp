#include "instance.hpp"
#include "artifact/instance.hpp"
#include "artifact/sets.hpp"
#include "character/characters.hpp"
#include "formula/artifact.hpp"
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
		auto &val = loadout.artifact.options.insert({setKey, {}}).first->second;
		set.getOptions(val);
	}
}

void Character::Instance::getArtifactStats() {
	static std::unordered_map<Artifact::SetKey, uint8_t> occurences{};
	loadout.artifact.set = std::nullopt;
	loadout.artifact.currentOptions = std::nullopt;

	for (const auto &[artPtr, index]: std::views::zip(Stats::Artifact::Slotted::getMembers(), std::views::iota(2, 7))) {
		auto artId = std::invoke(artPtr, loadout.artifact.equipped);
		if (!artId.has_value()) continue;
		auto &artifact = Store::artifacts.at(artId.value());
		occurences[artifact.set]++;
		loadout.artifact.sheet.preMods.fromStat(artifact.mainStat).modifiers.at(index) = Formula::ArtifactMainStat(artifact.mainStat, artifact.level);
		for (auto &subStat: artifact.subStats) {
			if (!subStat.has_value()) continue;
			loadout.artifact.sheet.preMods.fromStat(subStat->stat).modifiers.at(index) = Formula::ArtifactSubStat(subStat.value());
		}
	}
	for (auto &occurence: occurences) {
		if (occurence.second >= 2) {
			// Cache the ArtifactData to avoid doing two accesses
			const Artifact::Set &artifactData = Artifact::sets.at(occurence.first);
			Stats::setupModifiers(artifactData.data.twoPcMods.preMod, loadout.artifact.sheet.preMods, 0);
			Stats::setupModifiers(artifactData.data.twoPcMods.postMod, loadout.artifact.sheet.postMods, 0);
			Stats::setupModifiers(artifactData.data.twoPcMods.teamPreMod, loadout.artifact.sheet.teamPreMods, 0);
			Stats::setupModifiers(artifactData.data.twoPcMods.teamPostMod, loadout.artifact.sheet.teamPostMods, 0);
			Stats::setupModifiers(artifactData.data.twoPcMods.enemy, loadout.artifact.sheet.enemySheet, 0);
			Stats::setupTalents(artifactData.data.twoPcMods.talents, loadout.artifact.sheet.talents, 0);
			Stats::setupTalents(artifactData.data.twoPcMods.teamTalents, loadout.artifact.sheet.teamTalents, 0);
			// The second check should not be be outside since a four set can only happen
			// only if there is a two set
			if (occurence.second >= 4) {
				loadout.artifact.currentOptions = std::ref(loadout.artifact.options.at(occurence.first));
				loadout.artifact.set = Artifact::sets.at(occurence.first);
				Stats::setupModifiers(artifactData.data.fourPcMods.preMod, loadout.artifact.sheet.preMods, 1);
				Stats::setupModifiers(artifactData.data.fourPcMods.postMod, loadout.artifact.sheet.postMods, 1);
				Stats::setupModifiers(artifactData.data.fourPcMods.teamPreMod, loadout.artifact.sheet.teamPreMods, 1);
				Stats::setupModifiers(artifactData.data.fourPcMods.teamPostMod, loadout.artifact.sheet.teamPostMods, 1);
				Stats::setupModifiers(artifactData.data.fourPcMods.enemy, loadout.artifact.sheet.enemySheet, 1);
				Stats::setupTalents(artifactData.data.fourPcMods.talents, loadout.artifact.sheet.talents, 1);
				Stats::setupTalents(artifactData.data.fourPcMods.teamTalents, loadout.artifact.sheet.teamTalents, 1);
			}
		}
	}
	occurences.clear();
}
