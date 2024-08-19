#include "instance.hpp"
#include "artifact/instance.hpp"
#include "artifact/sets.hpp"
#include "character/characters.hpp"
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
	character.getConds(stats.character.conditionals, stats.character);

	Stats::setupModifiers(character.data.mods.preMod, stats.character.sheet.stats.preMods, 0);
	Stats::setupModifiers(character.data.mods.postMod, stats.character.sheet.stats.postMods, 0);

	stats.character.sheet.init(stats);
}

void Character::Instance::getArtifactStats() {
	static std::unordered_map<Artifact::SetKey, uint8_t> occurences{};
	stats.artifact.conditionals.clear();
	// FIXME: also clear sheet
	stats.artifact.set = std::nullopt;

	for (auto [artPtr, index]: std::views::zip(Stats::Artifact::Slotted::getMembers(), std::views::iota(2, 7))) {
		auto artId = std::invoke(artPtr, stats.artifact.equipped);
		if (artId == 0) continue;
		auto &artifact = Store::artifacts.at(artId);
		occurences[artifact.set]++;
		stats.artifact.sheet.stats.preMods.fromStat(artifact.mainStat).modifiers.at(index) = Formula::ArtifactMainStat(artifact.mainStat, artifact.level);
		for (auto &subStat: artifact.subStats) {
			stats.artifact.sheet.stats.preMods.fromStat(subStat.stat).modifiers.at(index) = Formula::ArtifactSubStat(subStat);
		}
	}
	for (auto &occurence: occurences) {
		if (occurence.second >= 2) {
			// Cache the ArtifactData to avoid doing two accesses
			const Artifact::Set &artifactData = Artifact::sets.at(occurence.first);
			Stats::setupModifiers(artifactData.data.twoPcMods.preMod, stats.artifact.sheet.stats.preMods, 0);
			// The second check should not be be outside since a four set can only happen
			// only if there is a two set
			if (occurence.second >= 4) {
				stats.artifact.set = Artifact::sets.at(occurence.first);
				artifactData.getConds(stats.artifact.conditionals, stats.artifact);
				Stats::setupModifiers(artifactData.data.fourPcMods.preMod, stats.artifact.sheet.stats.preMods, 1);
				Stats::setupModifiers(artifactData.data.fourPcMods.postMod, stats.artifact.sheet.stats.postMods, 1);
			}
		}
	}
	occurences.clear();
}
