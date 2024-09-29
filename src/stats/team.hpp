#pragma once

#include "array"
#include "formula/node.hpp"


namespace Character {
	struct Instance;
}

namespace Stats {
	struct Loadout;

	struct Team {
		std::array<::Character::Instance*, 4> characters{};
		Formula::ElementNode infusion;

		Team();
	};
}// namespace Stats