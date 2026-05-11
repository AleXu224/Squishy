#include "store.hpp"

#include "glaze/glaze.hpp"// IWYU pragma: keep
// #include "serialization/save/key.hpp"// IWYU pragma: keep
#include "serialization/save/save.hpp"

Agent::Instance &Store::createAgent(Agent::DataKey dataKey) {
	++lastAgentId;
	auto entry = agents.emplace(lastAgentId, Agent::Instance(lastAgentId, dataKey));

	return entry.first->second;
}

Engine::Instance &Store::createEngine(Engine::DataKey dataKey) {
	++lastEngineId;
	auto entry = engines.emplace(lastEngineId, Engine::Instance(dataKey, lastEngineId));

	return entry.first->second;
}

Disc::Instance &Store::createDisc(Disc::SetKey setKey) {
	++lastDiscId;
	auto entry = discs.emplace(lastDiscId, Disc::Instance{.key = lastDiscId, .set = setKey});

	return entry.first->second;
}

Team::Instance &Store::createTeam(std::string_view name) {
	++lastTeamId;
	auto entry = teams.emplace(lastTeamId, Team::Instance{.instanceKey = lastTeamId});

	return entry.first->second;
}

namespace {
	Serialization::Save::Save save() {
		std::vector<Serialization::Save::Disc> retDiscs{};
		retDiscs.reserve(::Store::discs.size());
		for (const auto &[_, discInstance]: ::Store::discs) {
			retDiscs.emplace_back(Serialization::Save::Disc::fromInstance(discInstance));
		}

		std::vector<Serialization::Save::Engine> retEngines{};
		retEngines.reserve(::Store::engines.size());
		for (const auto &[_, engineInstance]: ::Store::engines) {
			retEngines.emplace_back(Serialization::Save::Engine::fromInstance(engineInstance));
		}

		std::vector<Serialization::Save::Agent> retAgents{};
		retAgents.reserve(::Store::agents.size());
		for (const auto &[_, agentInstance]: ::Store::agents) {
			retAgents.emplace_back(Serialization::Save::Agent::fromInstance(agentInstance));
		}

		std::vector<Serialization::Save::Team> retTeams{};
		retTeams.reserve(::Store::teams.size());
		for (const auto &[_, teamInstance]: ::Store::teams) {
			retTeams.emplace_back(Serialization::Save::Team::fromInstance(teamInstance));
		}

		return Serialization::Save::Save{
			.discs = retDiscs,
			.engines = retEngines,
			.agents = retAgents,
			.teams = retTeams,
			.window{
				.width = ::Store::windowWidth,
				.height = ::Store::windowHeight,
				.maximized = ::Store::maximized,
			},
		};
	}

	void load(const Serialization::Save::Save &save) {
		::Store::discs.clear();
		::Store::engines.clear();
		::Store::agents.clear();
		::Store::teams.clear();
		::Store::enemies.clear();

		uint32_t maxDiscKey = 1;
		for (const auto &disc: save.discs) {
			maxDiscKey = std::max(maxDiscKey, disc.instanceKey.key);
			::Store::discs.emplace(disc.instanceKey, disc.toInstance());
		}
		::Store::lastDiscId = maxDiscKey;

		uint32_t maxEngineKey = 1;
		for (const auto &engine: save.engines) {
			maxEngineKey = std::max(maxEngineKey, engine.instanceKey.key);
			::Store::engines.emplace(engine.instanceKey, engine.toInstance());
		}
		::Store::lastEngineId = maxEngineKey;

		uint32_t maxAgentKey = 1;
		uint32_t maxComboKey = 1;
		for (const auto &agent: save.agents) {
			maxAgentKey = std::max(maxAgentKey, agent.instanceKey.key);
			for (const auto &combo: agent.combos) {
				maxComboKey = std::max(maxComboKey, combo.instanceKey.key);
			}
			::Store::agents.emplace(agent.instanceKey, agent.toInstance());
		}
		::Store::lastAgentId = maxAgentKey;
		::Store::lastComboId = maxComboKey;

		uint32_t maxTeamKey = 1;
		for (const auto &team: save.teams) {
			maxTeamKey = std::max(maxTeamKey, team.instanceKey.key);
			::Store::teams.emplace(team.instanceKey, team.toInstance());
		}
		::Store::lastTeamId = maxTeamKey;

		::Store::windowWidth = save.window.width;
		::Store::windowHeight = save.window.height;
		::Store::maximized = save.window.maximized;
	}
}// namespace

void Store::saveToFile(const std::string &strpath) {
	std::filesystem::path path = strpath;
	if (!path.has_filename()) throw std::runtime_error("Path must point to a file");
	std::filesystem::create_directory(path.parent_path());
	auto file = std::ofstream(path, std::ofstream::binary);
	{
		// Using json at the moment for debugability
		std::string buff{};
		auto d = glz::write<glz::opts{.prettify = false}>(save(), buff);
		if (d) {
			std::println("Failed saving");
		}
		file << buff;
	}
	file.close();
}

void Store::loadFromFile(const std::string &strpath) {
	std::filesystem::path path = strpath;
	if (!path.has_filename()) throw std::runtime_error("Path must point to a file");
	auto file = std::ifstream(path, std::ofstream::binary);
	if (!file.is_open()) return;

	std::stringstream ss{};
	ss << file.rdbuf();
	Serialization::Save::Save saveData;
	auto d = glz::read<glz::opts{.error_on_unknown_keys = false}>(saveData, ss.str());
	if (d) {
		std::println("Failed loading save file:");
		std::println("{}", d.custom_error_message);
		std::println("{}", std::to_underlying(d.ec));
		return;
	}
	file.close();

	load(saveData);
}

extern Serialization::Zod::IZOD Store::saveToZOD() {
	Serialization::Zod::IZOD ret{};

	std::vector<Serialization::Zod::IDisc> discs;
	for (const auto &[_, disc]: ::Store::discs) {
		discs.emplace_back(Serialization::Zod::IDisc::fromInstance(disc));
	}

	std::vector<Serialization::Zod::IEngine> engines;
	for (const auto &[_, engine]: ::Store::engines) {
		auto weps = Serialization::Zod::IEngine::fromInstance(engine);
		engines.insert(engines.end(), weps.begin(), weps.end());
	}

	std::vector<Serialization::Zod::IAgent> agents;
	for (const auto &[_, agent]: ::Store::agents) {
		agents.emplace_back(Serialization::Zod::IAgent::fromInstance(agent));
	}

	ret.discs = discs;
	ret.wengines = engines;
	ret.characters = agents;

	return ret;
}

extern void Store::loadFromZOD(const Serialization::Zod::IZOD &data) {
	if (data.characters.has_value()) {
		for (const auto &agent: data.characters.value()) {
			auto storedAgent = agent.isAlreadyStored();
			if (!storedAgent) storedAgent = agent.createInstance();
			if (!storedAgent) {
				std::println("{}", storedAgent.error());
				continue;
			}

			agent.writeToInstance(storedAgent.value());
		}
		agentListUpdateEvent.notify();
	}
	if (data.wengines.has_value()) {
		for (const auto &engine: data.wengines.value()) {
			auto storedEngine = engine.isAlreadyStored();
			if (!storedEngine) storedEngine = engine.createInstance();
			if (!storedEngine) {
				std::println("{}", storedEngine.error());
				continue;
			}

			engine.writeToInstance(storedEngine.value());
		}
		engineListUpdateEvent.notify();
	}
	if (data.discs.has_value()) {
		for (auto &&[key, disc]: ::Store::discs) {
			disc.unequip();
		}
		::Store::discs.clear();
		for (const auto &disc: data.discs.value()) {
			auto storedDisc = disc.createInstance();
			if (!storedDisc) {
				std::println("{}", storedDisc.error());
				continue;
			}
			disc.writeToInstance(storedDisc.value());
		}
		discListUpdateEvent.notify();
	}
}
