#include "IAgent.hpp"

#include "agent/agents.hpp"
#include "store.hpp"

Serialization::Zod::IAgent Serialization::Zod::IAgent::fromInstance(const Agent::Instance &agent) {
	return {
		.key = std::string(Agent::list.at(agent.dataKey).goodKey),
		.level = agent.state.stats.sheet.level,
		.mindscape = agent.state.stats.sheet.mindscape,
		.promotion = agent.state.stats.sheet.promotion,
		.core = static_cast<uint8_t>(agent.state.stats.sheet.skills.core.constant),
		.dodge = static_cast<uint8_t>(agent.state.stats.sheet.skills.dodge.constant + 1),
		.basic = static_cast<uint8_t>(agent.state.stats.sheet.skills.basic.constant + 1),
		.chain = static_cast<uint8_t>(agent.state.stats.sheet.skills.chain.constant + 1),
		.special = static_cast<uint8_t>(agent.state.stats.sheet.skills.special.constant + 1),
		.assist = static_cast<uint8_t>(agent.state.stats.sheet.skills.assist.constant + 1),
		.potential = 0,
	};
}

std::expected<std::reference_wrapper<Agent::Instance>, std::string> Serialization::Zod::IAgent::createInstance() const {
	auto agentData = getData();
	if (!agentData) return std::unexpected(agentData.error());
	const auto &data = agentData.value().get();

	auto &instance = Store::createAgent(data.key);
	writeToInstance(instance);

	return instance;
}

std::expected<std::reference_wrapper<const Agent::Data>, std::string> Serialization::Zod::IAgent::getData() const {
	for (const auto &[_, data]: Agent::list) {
		if (data.goodKey == key) return data;
	}
	return std::unexpected(std::format("Data for \"{}\" not found", key));
}

std::expected<std::reference_wrapper<Agent::Instance>, std::string> Serialization::Zod::IAgent::isAlreadyStored() const {
	auto agentData = getData();
	if (!agentData) return std::unexpected(agentData.error());
	auto &data = agentData.value().get();
	for (auto &[_, agent]: ::Store::agents) {
		if (agent.dataKey == data.key) return std::ref(agent);
	}
	return std::unexpected("Agent not found");
}

void Serialization::Zod::IAgent::writeToInstance(Agent::Instance &agent) const {
	agent.state.stats.sheet.level = level;
	agent.state.stats.sheet.mindscape = mindscape;
	agent.state.stats.sheet.promotion = promotion;
	agent.state.stats.sheet.skills.core.constant = core;
	agent.state.stats.sheet.skills.dodge.constant = dodge - 1;
	agent.state.stats.sheet.skills.basic.constant = basic - 1;
	agent.state.stats.sheet.skills.chain.constant = chain - 1;
	agent.state.stats.sheet.skills.special.constant = special - 1;
	agent.state.stats.sheet.skills.assist.constant = assist - 1;
	// agent.state.stats.sheet.potential = potential;
}
