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
		.dodge = static_cast<uint8_t>(agent.state.stats.sheet.skills.dodge.constant),
		.basic = static_cast<uint8_t>(agent.state.stats.sheet.skills.basic.constant),
		.chain = static_cast<uint8_t>(agent.state.stats.sheet.skills.chain.constant),
		.special = static_cast<uint8_t>(agent.state.stats.sheet.skills.special.constant),
		.assist = static_cast<uint8_t>(agent.state.stats.sheet.skills.assist.constant),
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
	agent.state.stats.sheet.skills.dodge.constant = dodge;
	agent.state.stats.sheet.skills.basic.constant = basic;
	agent.state.stats.sheet.skills.chain.constant = chain;
	agent.state.stats.sheet.skills.special.constant = special;
	agent.state.stats.sheet.skills.assist.constant = assist;
	// agent.state.stats.sheet.potential = potential;
}
