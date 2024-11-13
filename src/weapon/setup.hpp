#pragma once

#include "data.hpp"                 // IWYU pragma: export
#include "formula/formula.hpp"      // IWYU pragma: export
#include "modifiers/total/total.hpp"// IWYU pragma: export
#include "node/atkNode.hpp"         // IWYU pragma: export
#include "node/infoNode.hpp"        // IWYU pragma: export


namespace Weapon {
	using IsActive = Formula::impl_IsActive<Stats::WeaponSheet>;
	using GetFloat = Formula::impl_GetFloat<Stats::WeaponSheet>;

	using SubStat = Stats::WeaponBase::SubStat;

	using Modifiers::total;
	using Modifiers::totalEnemy;
	using Modifiers::totalTalents;
};// namespace Weapon
using namespace Formula;
