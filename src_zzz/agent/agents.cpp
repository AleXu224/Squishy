#include "agents.hpp"

#include "agent/agents/Alice.hpp"
#include "agent/agents/Yuzuha.hpp"

extern void Agent::initAgents() {
	list = {
		{Agent::Datas::alice.key, Agent::Datas::alice},
		{Agent::Datas::yuzuha.key, Agent::Datas::yuzuha},
	};
}
