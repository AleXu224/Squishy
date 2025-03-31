#include "loadout.hpp"

#include "artifact/sets.hpp"
#include "character/data.hpp"
#include "weapon/data.hpp"


void Stats::Loadout::init() {
	options.clear();
	const auto &characterData = character.data;
	characterData.getOpts(options);

	const auto &weaponData = weapon->data;
	weaponData->getOpts(options);

	character.sheet.init(*this);

	for (const auto &[setKey, set]: ::Artifact::sets) {
		set.getOptions(options);
	}
}
