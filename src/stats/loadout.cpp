#include "loadout.hpp"

#include "artifact/sets.hpp"
#include "character/data.hpp"
#include "store.hpp"
#include "weapon/data.hpp"


void Stats::State::init() {
	options.clear();
	const auto &characterData = stats.data;
	characterData.getOpts(options);

	loadout().init(options);

	for (auto &loadout: loadouts) {
		loadout.init(options);
	}

	stats.sheet.init(*this);
}

void Stats::Loadout::init(Option::TypesMap &options) {
	if (weapon) {
		const auto &weaponData = weapon->data;
		weaponData->getOpts(options);
	}

	for (const auto &[setKey, set]: ::Artifact::sets) {
		set.getOptions(options);
	}
}

void Stats::Loadout::swapWeapon(::Weapon::InstanceKey weaponInstanceKey) {
	if (!weaponInstanceKey) return;

	auto &weapon = ::Store::weapons.at(weaponInstanceKey);
	this->weaponInstanceKey = weaponInstanceKey;
	this->weapon = &weapon.stats;
}
