#pragma once

#include "option/option.hpp"
#include "weaponSheet.hpp"


namespace Weapon {
	struct Data;
}

namespace Stats {
	struct Weapon {
		const ::Weapon::Data *data;
		Stats::WeaponSheet sheet;
		Option::WeaponMap options;

		Weapon(const ::Weapon::Data &data);
	};
}// namespace Stats