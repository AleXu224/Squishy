#pragma once

#include "array"
#include "formula/node.hpp"
#include "option/option.hpp"
#include "stats/sheet.hpp"
#include "stats/value.hpp"


namespace Character {
	struct Instance;
}

namespace Stats {
	struct Loadout;

	struct Team {
		std::array<::Character::Instance *, 4> characters{};
		Formula::ElementNode infusion;
		Formula::IntNode moonsignLevel;
		Stats::Sheet<Stats::Value<float, 1>> resonances{};
		Stats::EnemySheet<Stats::Value<float, 1>> resonancesEnemy{};
		Option::TypesMap options{};

		Team();

		[[nodiscard]] bool hasCharacter(::Character::InstanceKey key) const;
	};
}// namespace Stats