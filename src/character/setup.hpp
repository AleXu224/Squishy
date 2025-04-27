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


namespace Character {
	using IsActive = Formula::impl_IsActive;
	using GetFloat = Formula::impl_GetFloat;
	using GetInt = Formula::impl_GetInt;

	static constexpr struct Character {
		static constexpr Formula::CharacterLevel level{};
		static constexpr Formula::CharacterConstellation constellation{};
		static constexpr Formula::CharacterAscension ascension{};
	} character{};

	using EntryType = Utils::EntryType;

	static inline const auto &preMods = Modifiers::preMods();
	static inline const auto &total = Modifiers::total();
	static inline const auto &totalActive = Modifiers::totalActive();
	static inline const auto &totalEnemy = Modifiers::totalEnemy();
	static inline const auto &totalTalents = Modifiers::totalTalents();
};// namespace Character
using namespace Formula;