#pragma once

#include "array"
#include "formula/node.hpp"
#include "option/option.hpp"
#include "stats/sheet.hpp"
#include "stats/value.hpp"
#include "team/key.hpp"

namespace Character {
	struct Instance;
}

namespace Stats {
	struct Loadout;

	struct Team {
		::Team::InstanceKey instanceKey;
		std::array<::Character::Instance *, 4> characters{};
		Formula::ElementNode infusion;
		Formula::IntNode moonsignLevel;
		uint32_t activeCharacterIndex = 0;
		Stats::Sheet<Stats::Value<float, 1>> resonances{};
		Stats::EnemySheet<Stats::Value<float, 1>> resonancesEnemy{};
		Option::TypesMap options{};

		Team(::Team::InstanceKey);

		[[nodiscard]] bool hasCharacter(::Character::InstanceKey key) const;
	};
}// namespace Stats