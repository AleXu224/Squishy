#include "instance.hpp"

#include "engine/engines.hpp"
#include "store.hpp"


Engine::Instance::Instance(const Engine::DataKey &dataKey, const Engine::InstanceKey &instanceKey)
	: instanceKey(instanceKey),
	  stats(Engine::list.at(dataKey)) {}

void Engine::Instance::equipOn(Agent::InstanceKey instanceKey, std::optional<uint32_t> loadoutIndex) const {
	if (!instanceKey) return;

	auto &agent = ::Store::agents.at(instanceKey);
	auto &loadout = agent.state.loadoutByIndex(loadoutIndex);

	loadout.swapEngine(this->instanceKey);
	stats.data->getOpts(agent.state.options);
	agent.updateEvent.notify();
	updateEvent.notify();
}

std::vector<Agent::InstanceKey> Engine::Instance::equippedOn() const {
	std::vector<Agent::InstanceKey> ret;
	for (const auto &[key, _]: Store::agents) {
		if (isEquippedOn(key)) ret.emplace_back(key);
	}
	return ret;
}

bool Engine::Instance::isEquippedOn(Agent::InstanceKey instanceKey) const {
	if (!instanceKey) return false;

	auto &agent = Store::agents.at(instanceKey);

	if (agent.state.equippedLoadout.engineInstanceKey == this->instanceKey) return true;

	return false;
}

std::vector<Agent::InstanceKey> Engine::Instance::usedOn() const {
	std::vector<Agent::InstanceKey> ret;

	for (const auto &[key, _]: Store::agents) {
		if (isUsedOn(key)) ret.emplace_back(key);
	}

	return ret;
}

bool Engine::Instance::isUsedOn(Agent::InstanceKey instanceKey) const {
	if (!instanceKey) return false;

	auto &agent = Store::agents.at(instanceKey);

	if (agent.state.equippedLoadout.engineInstanceKey == this->instanceKey) return true;

	for (const auto &loadout: agent.state.loadouts) {
		if (loadout.engineInstanceKey == this->instanceKey) return true;
	}

	return false;
}
