#pragma once

#include "option/option.hpp"
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
		Option::CharacterMap options{};

		explicit Character(const ::Character::Data &data);
	};
}// namespace Stats
