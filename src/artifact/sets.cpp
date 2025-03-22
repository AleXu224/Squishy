#include "sets.hpp"

#include "artifact/sets/CrimsonWitchOfFlames.hpp"
#include "artifact/sets/DeepwoodMemories.hpp"
#include "artifact/sets/EmblemOfSeveredFate.hpp"
#include "artifact/sets/GildedDreams.hpp"
#include "artifact/sets/GoldenTroupe.hpp"
#include "artifact/sets/MarechausseeHunter.hpp"
#include "artifact/sets/ObsidianCodex.hpp"
#include "artifact/sets/ScrollOfTheHeroOfCinderCity.hpp"
#include "artifact/sets/ShimenawaSReminiscence.hpp"
#include "artifact/sets/ThunderingFury.hpp"
#include "artifact/sets/WandererSTroupe.hpp"


void Artifact::initArtifacts() {
	sets = {
		{Artifact::Sets::crimsonWitchOfFlames.key, Artifact::Sets::crimsonWitchOfFlames},
		{Artifact::Sets::deepwoodMemories.key, Artifact::Sets::deepwoodMemories},
		{Artifact::Sets::emblemOfSeveredFate.key, Artifact::Sets::emblemOfSeveredFate},
		{Artifact::Sets::gildedDreams.key, Artifact::Sets::gildedDreams},
		{Artifact::Sets::goldenTroupe.key, Artifact::Sets::goldenTroupe},
		{Artifact::Sets::marechausseeHunter.key, Artifact::Sets::marechausseeHunter},
		{Artifact::Sets::obsidianCodex.key, Artifact::Sets::obsidianCodex},
		{Artifact::Sets::scrollOfTheHeroOfCinderCity.key, Artifact::Sets::scrollOfTheHeroOfCinderCity},
		{Artifact::Sets::shimenawaSReminiscence.key, Artifact::Sets::shimenawaSReminiscence},
		{Artifact::Sets::thunderingFury.key, Artifact::Sets::thunderingFury},
		{Artifact::Sets::wandererSTroupe.key, Artifact::Sets::wandererSTroupe},
	};
}
