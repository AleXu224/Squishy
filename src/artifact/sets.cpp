#include "sets.hpp"

#include "artifact/sets/CrimsonWitchOfFlames.hpp"
#include "artifact/sets/DeepwoodMemories.hpp"
#include "artifact/sets/GildedDreams.hpp"
#include "artifact/sets/GoldenTroupe.hpp"
#include "artifact/sets/MarechausseeHunter.hpp"
#include "artifact/sets/ObsidianCodex.hpp"
#include "artifact/sets/ScrollOfTheHeroOfCinderCity.hpp"


void Artifact::initArtifacts() {
	sets = {
		{Artifact::Sets::crimsonWitchOfFlames.key, Artifact::Sets::crimsonWitchOfFlames},
		{Artifact::Sets::deepwoodMemories.key, Artifact::Sets::deepwoodMemories},
		{Artifact::Sets::gildedDreams.key, Artifact::Sets::gildedDreams},
		{Artifact::Sets::goldenTroupe.key, Artifact::Sets::goldenTroupe},
		{Artifact::Sets::marechausseeHunter.key, Artifact::Sets::marechausseeHunter},
		{Artifact::Sets::obsidianCodex.key, Artifact::Sets::obsidianCodex},
		{Artifact::Sets::scrollOfTheHeroOfCinderCity.key, Artifact::Sets::scrollOfTheHeroOfCinderCity},
	};
}
