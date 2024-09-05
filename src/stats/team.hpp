#pragma once

#include "Talents.hpp"
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

		Sheet<Value<float, 1>> preMods{};
		Sheet<Value<float, 1>> postMods{};
		Talents<Value<uint8_t, 1>> talents{};

		Formula::ElementNode infusion;

		Team();
	};
}// namespace Stats