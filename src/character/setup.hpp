#pragma once

#include "data.hpp"                 // IWYU pragma: export
#include "formula/formula.hpp"      // IWYU pragma: export
#include "modifiers/total/total.hpp"// IWYU pragma: export
#include "node/atkNode.hpp"         // IWYU pragma: export
#include "node/healNode.hpp"        // IWYU pragma: export
#include "node/infoNode.hpp"        // IWYU pragma: export


namespace Character {
	using IsActive = Formula::impl_IsActive<Stats::CharacterSheet>;
	using GetFloat = Formula::impl_GetFloat<Stats::CharacterSheet>;
	using GetInt = Formula::impl_GetInt<Stats::CharacterSheet>;

	static constexpr struct Character {
		static constexpr Formula::CharacterLevel level{};
		static constexpr Formula::CharacterConstellation constellation{};
		static constexpr Formula::CharacterAscension ascension{};
	} character{};

	using Modifiers::total;
	using Modifiers::totalEnemy;
	using Modifiers::totalTalents;
};// namespace Character
using namespace Formula;