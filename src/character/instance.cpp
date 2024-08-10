#include "instance.hpp"
#include "artifact/instance.hpp"
#include "artifact/sets.hpp"
#include "character/characters.hpp"
#include "store.hpp"


Character::Instance::Instance(const Key &key, const Weapon::Key &weaponKey)
	: key(key),
	  weaponKey(weaponKey),
	  stats(Stats::Sheet{
		  .character = Stats::Character(Character::list.at(key)),
		  .weapon = Store::weapons.at(weaponKey).stats,
		  .artifact{},
	  }) {

	const auto &character = Character::list.at(key);
	character.getConds(stats.character.conditionals, stats.character);
	character.getMods(stats.character.conditionals, stats.character);

	stats.character.sheet.init(stats);
}

void Character::Instance::getArtifactStats() {
	static std::unordered_map<Artifact::SetKey, uint8_t> occurences{};
	stats.artifact.conditionals.clear();
	// FIXME: also clear sheet
	stats.artifact.set = std::nullopt;

	for (auto &artPtr: Stats::Artifact::Slotted::getMembers()) {
		auto artId = std::invoke(artPtr, stats.artifact.equipped);
		if (artId == 0) continue;
		auto &artifact = Store::artifacts.at(artId);
		occurences[artifact.set]++;
		Stats::addModifier(
			stats.artifact.sheet.fromStat(artifact.mainStat),
			Formula::Custom(
				"Artifact Mainstat", [mainStat = artifact.mainStat, level = artifact.level](const Stats::Sheet &) -> float {
					return Stats::Values::mainStat.at(mainStat).at(level);
				},
				Stats::isPercentage(artifact.mainStat)
			)
		);
		for (auto &subStat: artifact.subStats) {
			Stats::addModifier(
				stats.artifact.sheet.fromStat(subStat.stat),
				Formula::Custom(
					"Artifact Substat", [subStat = subStat](const Stats::Sheet &) {
						return subStat.value;
					},
					Stats::isPercentage(subStat.stat)
				)
			);
		}
	}
	for (auto &occurence: occurences) {
		if (occurence.second >= 2) {
			// Cache the ArtifactData to avoid doing two accesses
			const Artifact::Set &artifactData = Artifact::sets.at(occurence.first);
			artifactData.getModsTwo(stats.artifact);
			// The second check should not be be outside since a four set can only happen
			// only if there is a two set
			if (occurence.second >= 4) {
				stats.artifact.set = Artifact::sets.at(occurence.first);
				artifactData.getConds(stats.artifact.conditionals, stats.artifact);
				artifactData.getModsFour(stats.artifact);
			}
		}
	}
	occurences.clear();
}
