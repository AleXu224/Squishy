#include "sets.hpp"

#include "artifact/sets/GildedDreams.hpp"

extern void Artifact::initArtifacts() {
	sets = {
		{Artifact::Sets::gildedDreams.key, Artifact::Sets::gildedDreams},
	};

	for (auto &set: sets) {
		set.second.nodes = set.second.getNodes();
	}
}
