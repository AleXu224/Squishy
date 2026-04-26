#pragma once

#include "agent/key.hpp"
#include "array"
#include "option/option.hpp"
#include "stats/sheet.hpp"
#include "stats/value.hpp"


namespace Agent {
	struct Instance;
}

namespace Stats {
	struct Loadout;

	struct Team {
		std::array<::Agent::Instance *, 3> agents{};
		uint32_t activeAgentIndex = 0;
		Stats::Sheet<Stats::Value<float, 1>> resonances{};
		Stats::EnemySheet<Stats::Value<float, 1>> resonancesEnemy{};
		Option::TypesMap options{};

		Team();

		[[nodiscard]] bool hasAgent(::Agent::InstanceKey key) const;
	};
}// namespace Stats