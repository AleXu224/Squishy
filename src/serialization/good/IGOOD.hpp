#pragma once

#include "IArtifact.hpp"
#include "ICharacter.hpp"
#include "IWeapon.hpp"
#include "cstdint"
#include "string"
#include "vector"


namespace Serialization::Good {
	struct IGOOD {
		std::string format = "GOOD";
		uint8_t version = 6;
		std::string source = "Squishy";
		std::optional<std::vector<ICharacter>> characters = std::nullopt;
		std::optional<std::vector<IArtifact>> artifacts = std::nullopt;
		std::optional<std::vector<IWeapon>> weapons = std::nullopt;
	};
}// namespace Serialization::Good