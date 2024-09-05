#pragma once

#include "data.hpp"           // IWYU pragma: export
#include "formula/formula.hpp"// IWYU pragma: export
#include "node/atkNode.hpp"   // IWYU pragma: export

namespace Character {
	using IsActive = Formula::impl_IsActive<Stats::CharacterSheet>;
	using GetFloat = Formula::impl_GetFloat<Stats::CharacterSheet>;
};// namespace Character
using namespace Formula;