#include "sets.hpp"

#include "artifact/sets/GildedDreams.hpp"
#include "artifact/sets/GoldenTroupe.hpp"

void Artifact::initArtifacts() {
	sets = {
		{Artifact::Sets::gildedDreams.key, Artifact::Sets::gildedDreams},
		{Artifact::Sets::goldenTroupe.key, Artifact::Sets::goldenTroupe},
	};
}
