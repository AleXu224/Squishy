#pragma once

#include "data.hpp"

#include "unordered_map"

namespace Agent {
	inline std::unordered_map<Agent::DataKey, const Agent::Data &> list;

	extern void initAgents();
}// namespace Agent
