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

		Sheet<Value<Formula::Context, 1>> preMods{};
		Sheet<Value<Formula::Context, 1>> postMods{};

		Formula::ElementNode infusion;

		Team();
	};
}// namespace Stats