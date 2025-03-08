#pragma once

#include "stats/artifact.hpp"
#include "stats/character.hpp"
#include "stats/weapon.hpp"


namespace Stats {
	struct Loadout {
		Stats::Character character;
		Stats::Weapon *weapon;
		Stats::Artifact artifact;

		void init();
	};
}// namespace Stats