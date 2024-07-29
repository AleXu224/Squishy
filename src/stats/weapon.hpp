#pragma once

#include "weaponBase.hpp"
#include "weaponSheet.hpp"

namespace Stats {
	struct Weapon {
		const Stats::WeaponBase &base;
		Stats::WeaponSheet sheet;


		explicit Weapon(const WeaponBase &base) : base(base), sheet(base) {}
	};
}// namespace Stats