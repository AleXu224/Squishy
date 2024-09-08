#pragma once

#include "artifact/instance.hpp"
#include "character/instance.hpp"
#include "enemy/instance.hpp"
#include "stats/team.hpp"
#include "team/instance.hpp"
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
	inline std::unordered_map<Character::InstanceKey, Character::Instance> characters{};
	inline std::unordered_map<Team::Key, Team::Instance> teams{};
	inline std::unordered_map<Enemy::Key, Enemy::Instance> enemies{};
	inline std::unordered_map<Weapon::InstanceKey, Weapon::Instance> weapons{};
	inline std::unordered_map<Artifact::InstanceKey, Artifact::Instance> artifacts{};

	inline uint32_t lastId;
}// namespace Store
