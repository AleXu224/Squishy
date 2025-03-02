#pragma once

#include "ISubstat.hpp"
#include "cstdint"
#include "string"
#include "vector"


namespace Serialization::Good {
	struct IArtifact {
		std::string setKey;
		std::string slotKey;
		uint8_t level;
		uint8_t rarity;
		std::string mainStatKey;
		std::string location;
		bool lock = false;
        std::vector<ISubstat> substats;
	};
}// namespace Serialization::Good