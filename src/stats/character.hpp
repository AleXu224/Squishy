#pragma once

#include "stats/characterBase.hpp"
#include "stats/characterSheet.hpp"

namespace Stats {
	class Character {
	public:
		const Stats::CharacterBase &base;
		Stats::CharacterSheet sheet;

		explicit Character(const Stats::CharacterBase &base)
			: base(base), sheet{base} {}
	};

}// namespace Stats
