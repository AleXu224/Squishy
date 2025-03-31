#include "character.hpp"

Serialization::Save::Character Serialization::Save::Character::fromInstance(const ::Character::Instance &character) {
	return {
		.instanceKey = character.instanceKey,
		.dataKey = character.dataKey,
		.weaponInstanceKey = character.weaponInstanceKey,
		.constellation = character.loadout.character.sheet.constellation,
		.level = character.loadout.character.sheet.level,
		.ascension = character.loadout.character.sheet.ascension,
		.normalLevel = character.loadout.character.sheet.talents.normal.constant,
		.skillLevel = character.loadout.character.sheet.talents.skill.constant,
		.burstLevel = character.loadout.character.sheet.talents.burst.constant,
		.artifactFlower = character.loadout.artifact.equipped.flower,
		.artifactPlume = character.loadout.artifact.equipped.plume,
		.artifactSands = character.loadout.artifact.equipped.sands,
		.artifactGoblet = character.loadout.artifact.equipped.goblet,
		.artifactCirclet = character.loadout.artifact.equipped.circlet,
		.options = optionsFromInstance(character.loadout.options),
		.combos = comboFromInstance(character.combos),
	};
}

::Character::Instance Serialization::Save::Character::toInstance() const {
	auto instance = ::Character::Instance(instanceKey, dataKey, weaponInstanceKey);
	instance.loadout.character.sheet.constellation = constellation;
	instance.loadout.character.sheet.level = level;
	instance.loadout.character.sheet.ascension = ascension;
	instance.loadout.character.sheet.talents.normal.constant = normalLevel;
	instance.loadout.character.sheet.talents.skill.constant = skillLevel;
	instance.loadout.character.sheet.talents.burst.constant = burstLevel;
	instance.loadout.artifact.equipped.flower = artifactFlower;
	instance.loadout.artifact.equipped.plume = artifactPlume;
	instance.loadout.artifact.equipped.sands = artifactSands;
	instance.loadout.artifact.equipped.goblet = artifactGoblet;
	instance.loadout.artifact.equipped.circlet = artifactCirclet;

	optionsToInstance(options, instance.loadout.options);
	instance.combos = comboToInstance(combos);

	instance.loadout.artifact.refreshStats();

	return instance;
}
