#include "IEngine.hpp"

#include "agent/data.hpp"
#include "engine/data.hpp"
#include "engine/engines.hpp"
#include "store.hpp"


std::vector<Serialization::Zod::IEngine> Serialization::Zod::IEngine::fromInstance(const Engine::Instance &engine) {
	auto equippedAgents = [&]() {
		std::vector<std::string_view> ret;

		for (const auto &agentKey: engine.equippedOn()) {
			ret.emplace_back(Store::agents.at(agentKey).state.stats.data.goodKey);
		}

		// Add an empty string for unequipped engines
		if (ret.empty()) ret.emplace_back("");

		return ret;
	}();

	std::vector<Serialization::Zod::IEngine> ret;

	// Engines works differently here compared to how they are in the ZOD format
	// the ZOD format assumes a engine can only be equipped in one place, while
	// here they can be equipped by multiple agents at once
	// Because of this we have to create multiple copies of this engine for every
	// instance of this engine being equipped
	for (const auto &zodKey: equippedAgents) {
		ret.emplace_back(Serialization::Zod::IEngine{
			.key{engine.stats.data->goodKey},
			.level = engine.stats.sheet.level,
			.modification = engine.stats.sheet.modification,
			.phase = engine.stats.sheet.overclock,
			.location{zodKey},
		});
	}

	return ret;
}

std::expected<std::reference_wrapper<Engine::Instance>, std::string> Serialization::Zod::IEngine::createInstance() const {
	auto engineData = getData();
	if (!engineData) return std::unexpected(engineData.error());
	const auto &data = engineData.value();

	auto &engine = Store::createEngine(data.get().key);
	writeToInstance(engine);
	return std::ref(engine);
}

std::expected<std::reference_wrapper<const Engine::Data>, std::string> Serialization::Zod::IEngine::getData() const {
	for (const auto &data: Engine::list) {
		if (data.second.goodKey == key) return std::ref(data.second);
	}
	return std::unexpected(std::format("Data for \"{}\" not found", key));
}

std::expected<std::reference_wrapper<Engine::Instance>, std::string> Serialization::Zod::IEngine::isAlreadyStored() const {
	for (auto &entry: Store::engines) {
		if (entry.second.stats.data->goodKey == key) return std::ref(entry.second);
	}
	return std::unexpected("Engine not found");
}

void Serialization::Zod::IEngine::writeToInstance(Engine::Instance &engine) const {
	engine.stats.sheet.level = level;
	engine.stats.sheet.modification = modification;
	engine.stats.sheet.overclock = phase;

	Agent::InstanceKey equippedAgent{};
	for (const auto &[_, agent]: ::Store::agents) {
		if (agent.state.stats.data.goodKey == location && agent.state.equippedLoadout.engineInstanceKey != engine.instanceKey) {
			equippedAgent = agent.instanceKey;
			break;
		}
	}
	if (equippedAgent) engine.equipOn(equippedAgent);
}
