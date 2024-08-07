#pragma once

#include "weapon/data.hpp"
#include <unordered_map>

namespace Weapon {
	inline std::unordered_map<Weapon::Key, const Weapon::Data &> list;

	extern void initWeapons();
}// namespace Weapon
