#pragma once

#include "artifact.hpp"
#include "character.hpp"
#include "team.hpp"
#include "weapon.hpp"


namespace Serialization::Save {
	struct Save {
		std::vector<Serialization::Save::Artifact> artifacts;
		std::vector<Serialization::Save::Weapon> weapons;
		std::vector<Serialization::Save::Character> characters;
		std::vector<Serialization::Save::Team> teams;
	};
}// namespace Serialization::Save