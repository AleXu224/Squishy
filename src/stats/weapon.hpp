#pragma once

#include "conditional/conditional.hpp"
#include "weaponBase.hpp"
#include "weaponSheet.hpp"


namespace Weapon {
	struct Data;
}

namespace Stats {
	struct Weapon {
		const ::Weapon::Data &data;
		const Stats::WeaponBase &base;
		Stats::WeaponSheet sheet;
		Conditional::WeaponMap conditionals{};

		explicit Weapon(const ::Weapon::Data &data);
	};
}// namespace Stats