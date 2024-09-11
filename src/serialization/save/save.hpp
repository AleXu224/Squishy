#pragma once

#include "cereal/cereal.hpp"

#include "cereal/types/vector.hpp"// IWYU pragma: export

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

		template<class Archive>
		void serialize(Archive &ar) {
			ar(
				CEREAL_NVP(artifacts),
				CEREAL_NVP(weapons),
				CEREAL_NVP(characters),
				CEREAL_NVP(teams)
			);
		}
	};
}// namespace Serialization::Save