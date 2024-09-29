#pragma once

#include "array"
#include "formula/node.hpp"
#include "optional"


namespace Character {
	struct Instance;
}

namespace Stats {
	struct Loadout;

	struct Team {
		std::array<std::optional<std::reference_wrapper<::Character::Instance>>, 4> characters{};
		Formula::ElementNode infusion;

		Team();
	};
}// namespace Stats