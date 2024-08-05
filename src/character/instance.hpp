#pragma once

#include "character/data.hpp"
#include "conditional/conditional.hpp"
#include "stats/character.hpp"
#include "weapon/data.hpp"


#include "array"


struct SavedCharData;

namespace Character {
	struct Instance {
		const Key key;
		Weapon::Key weaponKey;
		Stats::Character stats;
		Node::List nodes;

		// The array of artifacts in the order of flower, plume, sands, goblet, circlet
		// An artifact with an id of 0 means that the slot is empty
		std::array<unsigned int, 5> arts{0};

		// Conditionals will be stored in an unordered_map for fast access
		// the initial slow add of a new key will not be a problem since it is rarely done
		// An alternative could be doing it as a vector with an enum of keys
		// but that would get really troublesome to maintain
		// WARNING: keys should be added during the collection of modifiers to avoid
		// the penalty of checking if they exist on every modifier that uses them
		Conditional::ListMapped conditionals{};

		explicit Instance(const Key &key, const Weapon::Key &weaponKey);

		// Instance(const Key &key, SavedCharData &data);

		void collectStats();

		void getArtifactStats();

		void getArtifactModifiers();
	};
}// namespace Character
