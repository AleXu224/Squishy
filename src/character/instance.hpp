#pragma once

#include "observer.hpp"

#include "character/key.hpp"
#include "node/node.hpp"
#include "stats/sheet.hpp"
#include "weapon/key.hpp"

#include "array"


struct SavedCharData;

namespace Character {
	struct Instance {
		const Key key;
		Weapon::Key weaponKey;
		Stats::Sheet stats;
		Node::List nodes;
		squi::VoidObservable updateEvent{};

		// The array of artifacts in the order of flower, plume, sands, goblet, circlet
		// An artifact with an id of 0 means that the slot is empty
		std::array<unsigned int, 5> arts{0};

		explicit Instance(const Key &key, const Weapon::Key &weaponKey);

		// Instance(const Key &key, SavedCharData &data);

		void collectStats();

		void getArtifactStats();

		void getArtifactModifiers();
	};
}// namespace Character
