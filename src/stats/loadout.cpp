#include "loadout.hpp"

#include "artifact/sets.hpp"
#include "character/data.hpp"


void Stats::Loadout::init() {
	const auto &characterData = character.data;
	character.options.clear();
	characterData.getOpts(character.options);

	character.sheet.init(*this);

	for (const auto &[setKey, set]: ::Artifact::sets) {
		set.getOptions(artifact.options);
	}
}
