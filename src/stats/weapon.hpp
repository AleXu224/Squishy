#pragma once

#include "weaponSheet.hpp"


namespace Weapon {
	struct Data;
}

namespace Stats {
	struct Weapon {
		const ::Weapon::Data *data;
		Stats::WeaponSheet sheet;

		Weapon(const ::Weapon::Data &data);
	};
}// namespace Stats