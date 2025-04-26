#pragma once

#include "data.hpp"                 // IWYU pragma: export
#include "formula/formula.hpp"      // IWYU pragma: export
#include "modifiers/total/total.hpp"// IWYU pragma: export
#include "node/atkNode.hpp"         // IWYU pragma: export
#include "node/customAtkNode.hpp"   // IWYU pragma: export
#include "node/healNode.hpp"        // IWYU pragma: export
#include "node/infoNode.hpp"        // IWYU pragma: export
#include "node/shieldNode.hpp"      // IWYU pragma: export


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

	using Modifiers::preMods;
	using Modifiers::total;
	using Modifiers::totalActive;
	using Modifiers::totalEnemy;
	using Modifiers::totalTalents;
};// namespace Character
using namespace Formula;