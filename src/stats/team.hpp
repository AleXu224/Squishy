#pragma once

#include "array"
#include "optional"
#include "sheet.hpp"
#include "value.hpp"

namespace Character {
	struct Instance;
}

namespace Stats {
	struct Loadout;

	struct Team {
		std::array<std::optional<std::reference_wrapper<::Character::Instance>>, 4> characters{};

		Sheet<Value<Loadout, Team, 1>> preMods{};
		Sheet<Value<Loadout, Team, 1>> postMods{};

		Team();
	};
}// namespace Stats