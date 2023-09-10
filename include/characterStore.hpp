#pragma once
#include "ICharacterData.hpp"
#include "IWeaponData.hpp"
#include "artifact.hpp"
#include "character.hpp"
#include "memory"
#include <vector>

namespace Squishy {

	struct Store {
		// Data
		struct Data {
			static const std::vector<ICharacterData> characters;
			static const std::vector<IWeaponData> weapons;
		};

		static std::vector<std::shared_ptr<Character>> characters;
		static std::vector<std::shared_ptr<Artifact>> artifacts;
	};
}// namespace Squishy