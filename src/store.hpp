#pragma once

#include "artifact/instance.hpp"
#include "character/instance.hpp"
#include "unordered_map"
#include "weapon/instance.hpp"


// struct SavedCharData {
// 	unsigned short level;
// 	unsigned short ascension;
// 	WeaponKey weaponKey;
// 	unsigned short weaponLevel;
// 	unsigned short weaponAscension;
// 	std::array<unsigned int, 5> artifacts;
// };

namespace Store {
	inline std::unordered_map<Character::Key, Character::Instance> characters{};
	inline std::unordered_map<Weapon::Key, Weapon::Instance> weapons{};
	inline std::unordered_map<uint32_t, Artifact::Instance> artifacts{};

	inline uint32_t lastId;
}
