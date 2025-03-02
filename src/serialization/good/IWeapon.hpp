#pragma once

#include "string"
#include "cstdint"

namespace Serialization::Good {
	struct IWeapon {
        std::string key;
        uint8_t level;
        uint8_t ascension;
        uint8_t refinement;
        std::string location;
        bool lock = false;
	};
}// namespace Serialization::Good