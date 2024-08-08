#pragma once

#include "observer.hpp"

#include "character/key.hpp"
#include "stats/sheet.hpp"
#include "weapon/key.hpp"


namespace Character {
	struct Instance {
		const Key key;
		Weapon::Key weaponKey;
		Stats::Sheet stats;
		squi::VoidObservable updateEvent{};

		// The array of artifacts in the order of flower, plume, sands, goblet, circlet
		// An artifact with an id of 0 means that the slot is empty
		explicit Instance(const Key &key, const Weapon::Key &weaponKey);

		void getArtifactStats();
	};
}// namespace Character
