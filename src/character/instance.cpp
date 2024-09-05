#include "instance.hpp"
#include "artifact/instance.hpp"
#include "artifact/sets.hpp"
#include "character/characters.hpp"
#include "formula/artifact.hpp"
#include "formula/teamCharacter.hpp"
#include "store.hpp"


Character::Instance::Instance(const Key &key, const Weapon::Key &weaponKey)
	: key(key),
	  weaponKey(weaponKey),
	  stats(Stats::Loadout{
		  .character = Stats::Character(Character::list.at(key)),
		  .weapon = Store::weapons.at(weaponKey).stats,
		  .artifact{},
	  }) {

	const auto &character = Character::list.at(key);
	character.getOpts(stats.character.options);

	Stats::setupModifiers(character.data.mods.preMod, stats.character.sheet.preMods, 0);
	Stats::setupModifiers(character.data.mods.postMod, stats.character.sheet.postMods, 0);
	Stats::setupModifiers(character.data.mods.teamPreMod, stats.character.sheet.teamPreMods, 0);
	Stats::setupModifiers(character.data.mods.teamPostMod, stats.character.sheet.teamPostMods, 0);
	Stats::setupModifiers(character.data.mods.enemy, stats.character.sheet.enemySheet, 0);

	stats.character.sheet.infusion = Formula::CharacterTeamInfusion(character.data.mods.infusion);
	stats.character.sheet.teamInfusion = character.data.mods.teamInfusion;

	Stats::setupTalents(character.data.mods.talents, stats.character.sheet.talents, 0);
	Stats::setupTalents(character.data.mods.teamTalents, stats.character.sheet.teamTalents, 0);

	stats.character.sheet.init(stats);
}

void Character::Instance::getArtifactStats() {
	static std::unordered_map<Artifact::SetKey, uint8_t> occurences{};
	stats.artifact.options.clear();
	// FIXME: also clear sheet
	stats.artifact.set = std::nullopt;

	for (const auto &[artPtr, index]: std::views::zip(Stats::Artifact::Slotted::getMembers(), std::views::iota(2, 7))) {
		auto artId = std::invoke(artPtr, stats.artifact.equipped);
		if (artId == 0) continue;
		auto &artifact = Store::artifacts.at(artId);
		occurences[artifact.set]++;
		stats.artifact.sheet.preMods.fromStat(artifact.mainStat).modifiers.at(index) = Formula::ArtifactMainStat(artifact.mainStat, artifact.level);
		for (auto &subStat: artifact.subStats) {
			stats.artifact.sheet.preMods.fromStat(subStat.stat).modifiers.at(index) = Formula::ArtifactSubStat(subStat);
		}
	}
	for (auto &occurence: occurences) {
		if (occurence.second >= 2) {
			// Cache the ArtifactData to avoid doing two accesses
			const Artifact::Set &artifactData = Artifact::sets.at(occurence.first);
			Stats::setupModifiers(artifactData.data.twoPcMods.preMod, stats.artifact.sheet.preMods, 0);
			Stats::setupModifiers(artifactData.data.twoPcMods.postMod, stats.artifact.sheet.postMods, 0);
			Stats::setupModifiers(artifactData.data.twoPcMods.teamPreMod, stats.artifact.sheet.teamPreMods, 0);
			Stats::setupModifiers(artifactData.data.twoPcMods.teamPostMod, stats.artifact.sheet.teamPostMods, 0);
			Stats::setupModifiers(artifactData.data.twoPcMods.enemy, stats.artifact.sheet.enemySheet, 0);
			Stats::setupTalents(artifactData.data.twoPcMods.talents, stats.artifact.sheet.talents, 0);
			Stats::setupTalents(artifactData.data.twoPcMods.teamTalents, stats.artifact.sheet.teamTalents, 0);
			// The second check should not be be outside since a four set can only happen
			// only if there is a two set
			if (occurence.second >= 4) {
				stats.artifact.set = Artifact::sets.at(occurence.first);
				artifactData.getOptions(stats.artifact.options);
				Stats::setupModifiers(artifactData.data.fourPcMods.preMod, stats.artifact.sheet.preMods, 1);
				Stats::setupModifiers(artifactData.data.fourPcMods.postMod, stats.artifact.sheet.postMods, 1);
				Stats::setupModifiers(artifactData.data.fourPcMods.teamPreMod, stats.artifact.sheet.teamPreMods, 1);
				Stats::setupModifiers(artifactData.data.fourPcMods.teamPostMod, stats.artifact.sheet.teamPostMods, 1);
				Stats::setupModifiers(artifactData.data.fourPcMods.enemy, stats.artifact.sheet.enemySheet, 1);
				Stats::setupTalents(artifactData.data.fourPcMods.talents, stats.artifact.sheet.talents, 1);
				Stats::setupTalents(artifactData.data.fourPcMods.teamTalents, stats.artifact.sheet.teamTalents, 1);
			}
		}
	}
	occurences.clear();
}
