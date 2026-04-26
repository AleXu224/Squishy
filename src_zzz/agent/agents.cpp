#include "agents.hpp"

#include "agent/agents/Alice.hpp"

extern void Agent::initAgents() {
	list = {
		{Agent::Datas::alice.key, Agent::Datas::alice},
	};
}
