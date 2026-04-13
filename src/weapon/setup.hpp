#pragma once

#include "data.hpp"                  // IWYU pragma: export
#include "formula/formula.hpp"       // IWYU pragma: export
#include "modifiers/total/premod.hpp"// IWYU pragma: export
#include "modifiers/total/total.hpp" // IWYU pragma: export
#include "node/atkNode.hpp"          // IWYU pragma: export
#include "node/customAtkNode.hpp"    // IWYU pragma: export
#include "node/healNode.hpp"         // IWYU pragma: export
#include "node/infoNode.hpp"         // IWYU pragma: export
#include "node/shieldNode.hpp"       // IWYU pragma: export


namespace Weapon {
	static inline auto IsActive(const Utils::HashedString &name) {
		return Formula::impl_IsActive{.name = name};
	}
	static inline auto GetFloat(const Utils::HashedString &name, float defaultValue = 0.f) {
		return Formula::impl_GetFloat{.name = name, .defaultValue = defaultValue};
	}
	static inline auto GetInt(const Utils::HashedString &name, uint32_t defaultValue = 0) {
		return Formula::impl_GetInt{.name = name, .defaultValue = defaultValue};
	}
	static inline auto GetIndex(const Utils::HashedString &name, uint32_t defaultValue = 0) {
		return Formula::impl_GetIndex{.name = name, .defaultValue = defaultValue};
	}

	using SubStat = Stats::WeaponBase::SubStat;

	static inline const auto &preMods = Modifiers::preMods();
	static inline const auto &total = Modifiers::total();
	static inline const auto &totalEnemy = Modifiers::totalEnemy();
	static inline const auto &totalTalents = Modifiers::totalTalents();
};// namespace Weapon
using namespace Formula;
