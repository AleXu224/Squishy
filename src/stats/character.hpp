#pragma once

#include "conditional/conditional.hpp"
#include "stats/characterBase.hpp"
#include "stats/characterSheet.hpp"


namespace Character {
	struct Data;
}

namespace Stats {
	struct Character {
		const ::Character::Data &data;
		const Stats::CharacterBase &base;
		Stats::CharacterSheet sheet;
		Conditional::CharacterMap conditionals{};

		explicit Character(const ::Character::Data &data);
	};
}// namespace Stats
