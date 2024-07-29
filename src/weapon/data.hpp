#pragma once

#include "functional"
#include "stats/weaponBase.hpp"
#include "cstdint"

namespace Character {
	struct Instance;
}

namespace Weapon {
	using Key = uint32_t;
	
	struct Data {
		const Key key;
		const Stats::WeaponBase baseStats;
		const std::vector<std::vector<float>> multipliers;
		const std::function<void(Character::Instance &character)> collectModifiers;
	};
}// namespace Weapon
