#pragma once

#include "artifact/set.hpp"          // IWYU pragma: export
#include "formula/formula.hpp"       // IWYU pragma: export
#include "modifiers/total/premod.hpp"// IWYU pragma: export
#include "modifiers/total/total.hpp" // IWYU pragma: export
#include "node/atkNode.hpp"          // IWYU pragma: export
#include "node/customAtkNode.hpp"    // IWYU pragma: export
#include "node/healNode.hpp"         // IWYU pragma: export
#include "node/infoNode.hpp"         // IWYU pragma: export
#include "node/shieldNode.hpp"       // IWYU pragma: export


namespace Artifact {
	using IsActive = Formula::impl_IsActive;
	using GetFloat = Formula::impl_GetFloat;
	using GetInt = Formula::impl_GetInt;

	static constexpr struct Character {
		static constexpr Formula::CharacterLevel level{};
		static constexpr Formula::CharacterConstellation constellation{};
		static constexpr Formula::CharacterAscension ascension{};
	} character{};

	using EntryType = Utils::EntryType;

	inline const auto &preMods = Modifiers::preMods();
	inline const auto &total = Modifiers::total();
	inline const auto &totalActive = Modifiers::totalActive();
	inline const auto &totalEnemy = Modifiers::totalEnemy();
	inline const auto &totalTalents = Modifiers::totalTalents();
};// namespace Artifact
using namespace Formula;
