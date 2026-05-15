#include "agents.hpp"

#include "agent/agents/Alice.hpp"
#include "agent/agents/NangongYu.hpp"
#include "agent/agents/Yuzuha.hpp"


extern void Agent::initAgents() {
	list = {
		{Agent::Datas::alice.key, Agent::Datas::alice},
		{Agent::Datas::nangongYu.key, Agent::Datas::nangongYu},
		{Agent::Datas::yuzuha.key, Agent::Datas::yuzuha},
	};

	for (auto &[key, data]: list) {
		const_cast<Agent::Data &>(data).data = std::make_unique<Agent::Data::Setup>(data.setup());
	}
}
