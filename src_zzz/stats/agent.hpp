#pragma once

#include "stats/agentBase.hpp"
#include "stats/agentSheet.hpp"


namespace Agent {
	struct Data;
}

namespace Stats {
	struct Agent {
		const ::Agent::Data &data;
		const Stats::AgentBase &base;
		Stats::AgentSheet sheet;

		explicit Agent(const ::Agent::Data &data);
	};
}// namespace Stats
