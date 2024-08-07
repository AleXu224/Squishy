#pragma once

#include "stats/characterBase.hpp"
#include "stats/characterSheet.hpp"
#include "conditional/conditional.hpp"


namespace Stats {
	struct Character {
		const Stats::CharacterBase &base;
		Stats::CharacterSheet sheet;
		Conditional::CharacterMap conditionals{};

		explicit Character(const Stats::CharacterBase &base)
			: base(base), sheet{} {}
	};

}// namespace Stats
