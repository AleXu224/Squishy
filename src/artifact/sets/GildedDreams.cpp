#include "GildedDreams.hpp"

#include "character/instance.hpp"

const Artifact::Set Artifact::Sets::gildedDreams{
	.key = 15026,
	.name = "Gilded Dreams",
	.twoSet = [](Character::Instance &character) {
		character.stats.sheet.addArtifactModifier(Stat::em, [](const Stats::CharacterSheet &) {
			return 80.f;
		});
	},
	.fourSet = [](Character::Instance &character) {},
};