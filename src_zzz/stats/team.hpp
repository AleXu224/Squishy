#pragma once

#include "agent/key.hpp"
#include "array"
#include "option/option.hpp"
#include "stats/sheet.hpp"
#include "stats/value.hpp"
#include "team/key.hpp"


namespace Agent {
	struct Instance;
}

namespace Stats {
	struct Loadout;

	struct Team {
		::Team::InstanceKey instanceKey;
		std::array<::Agent::Instance *, 3> agents{};
		uint32_t activeAgentIndex = 0;
		Stats::Sheet<Stats::Value<float, 1>> resonances{};
		Stats::EnemySheet<Stats::Value<float, 1>> resonancesEnemy{};
		Option::TypesMap options{};

		Team(::Team::InstanceKey);

		[[nodiscard]] bool hasAgent(::Agent::InstanceKey key) const;
	};
}// namespace Stats