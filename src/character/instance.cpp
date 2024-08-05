#include "instance.hpp"
#include "artifact/instance.hpp"
#include "artifact/sets.hpp"
#include "store.hpp"
#include "character/characters.hpp"


Character::Instance::Instance(const Key &key, const Weapon::Key &weaponKey)
	: key(key),
	  weaponKey(weaponKey),
	  stats(Character::list.at(key).baseStats),
	  nodes(Character::list.at(key).getNodes(stats)),
	  conditionals(Character::list.at(key).getConds(stats)) {
	collectStats();

	const auto &character = Character::list.at(key);
	character.getMods(stats);
}

void Character::Instance::collectStats() {
	const auto &weapon = Store::weapons.at(weaponKey);
	const auto &characterData = Character::list.at(key);
	// weapon.collectStats(stats);
	weapon.data.collectModifiers(*this);
	characterData.modsSetup(Character::Data::ModsSetup{
		.stats = stats,
		.multipliers = characterData.multipliers,
	});

	stats.sheet.fromStat(stats.base.ascensionStat).modifiers.emplace_back([&base = stats.base](const Stats::CharacterSheet &sheet) {
		return base.getAscensionStatAt(sheet.ascension);
	});
	stats.sheet.baseAtk.modifiers.emplace_back([&base = stats.base](const Stats::CharacterSheet &sheet) {
		return base.getAscensionStatAt(sheet.ascension);
	});
	stats.sheet.baseAtk.modifiers.emplace_back([wepKey = weapon.data.key](const Stats::CharacterSheet &) {
		const auto &weapon = Store::weapons.at(wepKey);
		return weapon.stats.sheet.atk.getTotal(weapon.stats.sheet);
	});
	stats.sheet.fromStat(weapon.data.baseStats.substat.stat).modifiers.emplace_back([wepKey = weapon.data.key](const Stats::CharacterSheet &) {
		const auto &weapon = Store::weapons.at(wepKey);
		return weapon.stats.sheet.subStat.getTotal(weapon.stats.sheet);
	});
}

void Character::Instance::getArtifactStats() {
	for (auto &art: arts) {
		if (art == 0) continue;
		auto &artifact = Store::artifacts.at(art);
		stats.sheet.fromStat(artifact.mainStat).artifactModifiers.emplace_back([mainStat = artifact.mainStat, level = artifact.level](const Stats::CharacterSheet &) {
			return Stats::Values::mainStat.at(mainStat).at(level);
		});
		for (auto &subStat: artifact.subStats) {
			stats.sheet.fromStat(subStat.stat).artifactModifiers.emplace_back([subStat = subStat](const Stats::CharacterSheet &) {
				return subStat.value;
			});
		}
	}
}


void Character::Instance::getArtifactModifiers() {
	// stats.sheet.clearArtifactModifiers();
	// Possibility to instead use a vector preinitialized with all the artifact keys
	// vector<Artifactkey> occurences{ArtifactKey::last, 0>
	static std::unordered_map<Artifact::SetKey, unsigned short> occurences{};
	for (auto &artId: arts) {
		if (artId == 0) continue;
		// Not checking if the artifact id is valid for performance reasons
		auto &art = Store::artifacts.at(artId);
		occurences[art.set]++;
		// if (occurences.contains(art.set)) occurences.at(art.set)++;
		// else
		// 	occurences[art.setKey] = 1;
	}
	for (auto &occurence: occurences) {
		if (occurence.second >= 2) {
			// Cache the ArtifactData to avoid doing two accesses
			const Artifact::Set &artifactData = Artifact::sets.at(occurence.first);
			artifactData.twoSet(*this);
			// The second check should not be be outside since a four set can only happen
			// only if there is a two set
			if (occurence.second >= 4) artifactData.fourSet(*this);
		}
		// Set the value to zero instead of clearing since this is way faster
		// Clearing takes ~70 nanoseconds on my machine
		occurence.second = 0;
	}
}

// Character::Instance::Instance(const Key &key, SavedCharData &data)
// 	: key(key),
// 	  stats(Character::list.at(key).baseStats),
// 	  weapon(data.weaponKey),
// 	  data(Character::list.at(key)),
// 	  arts(data.artifacts) {

// 	stats.sheet.level = data.level;
// 	stats.sheet.ascension = data.ascension;
// 	stats.sheet.talents.skill = 10;
// 	stats.sheet.talents.burst = 10;
// 	weapon.stats.level = data.weaponLevel;
// 	weapon.stats.ascension = data.weaponAscension;
// 	// weapon.stats.refinement = data.weaponRefinement;
// 	collectStats();
// 	getArtifactModifiers();
// 	getArtifactStats();
// }
