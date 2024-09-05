#pragma once

#include "data.hpp"           // IWYU pragma: export
#include "formula/formula.hpp"// IWYU pragma: export
#include "node/atkNode.hpp"   // IWYU pragma: export

namespace Weapon {
	using IsActive = Formula::impl_IsActive<Stats::WeaponSheet>;
	using GetFloat = Formula::impl_GetFloat<Stats::WeaponSheet>;
};// namespace Weapon
using namespace Formula;
