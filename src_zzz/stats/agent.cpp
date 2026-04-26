#include "agent.hpp"

#include "agent/data.hpp"

Stats::Agent::Agent(const ::Agent::Data &data)
	: data(data), base(data.baseStats), sheet{} {}
