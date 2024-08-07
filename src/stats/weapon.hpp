#pragma once

#include "conditional/conditional.hpp"
#include "weaponBase.hpp"
#include "weaponSheet.hpp"


namespace Stats {
	struct Weapon {
		const Stats::WeaponBase &base;
		Stats::WeaponSheet sheet;
		Conditional::WeaponMap conditionals{};

		explicit Weapon(const WeaponBase &base) : base(base), sheet(base) {}
	};
}// namespace Stats