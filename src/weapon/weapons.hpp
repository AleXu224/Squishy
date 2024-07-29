#pragma once

#include "weapon/data.hpp"
#include "weapon/weapons/StaffOfTheScarletSands.hpp"
#include <unordered_map>

namespace Weapon {
	inline std::unordered_map<Weapon::Key, const Weapon::Data &> list;

	inline void initWeapons() {
		list = {
			{Weapon::Datas::staffOfTheScarletSands.key, Weapon::Datas::staffOfTheScarletSands},
		};
	}
}// namespace Weapon
