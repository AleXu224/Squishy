#pragma once

#include "observer.hpp"

#include "character/key.hpp"
#include "stats/loadout.hpp"
#include "weapon/key.hpp"


namespace Character {
	struct Instance {
		const Key key;
		Weapon::Key weaponKey;
		Stats::Loadout stats;
		squi::VoidObservable updateEvent{};

		explicit Instance(const Key &key, const Weapon::Key &weaponKey);

		void getArtifactStats();
	};
}// namespace Character
