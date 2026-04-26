#pragma once

#include "agent/instance.hpp"
#include "disc/instance.hpp"
#include "enemy/instance.hpp"
#include "engine/instance.hpp"
#include "stats/team.hpp"
#include "team/instance.hpp"
#include "unordered_map"


// #include "serialization/good/IGOOD.hpp"

namespace Store {
	inline std::unordered_map<Agent::InstanceKey, Agent::Instance> agents{};
	inline std::unordered_map<Team::InstanceKey, Team::Instance> teams{};
	inline std::unordered_map<Enemy::Key, Enemy::Instance> enemies{};
	inline std::unordered_map<Engine::InstanceKey, Engine::Instance> engines{};
	inline std::unordered_map<Disc::InstanceKey, Disc::Instance> discs{};

	static inline Team::Instance defaultTeam{
		.instanceKey{std::numeric_limits<uint32_t>::max()},
		.name = "Default team",
	};

	inline uint32_t lastDiscId = 1;
	inline uint32_t lastEngineId = 1;
	inline uint32_t lastAgentId = 1;
	inline uint32_t lastTeamId = 1;
	inline uint32_t lastComboId = 1;

	inline squi::VoidObservable discListUpdateEvent{};
	inline squi::VoidObservable engineListUpdateEvent{};
	inline squi::VoidObservable agentListUpdateEvent{};
	inline squi::VoidObservable teamListUpdateEvent{};

	inline uint32_t windowWidth = 1280;
	inline uint32_t windowHeight = 720;
	inline bool maximized = false;

	Agent::Instance &createAgent(Agent::DataKey dataKey, Engine::InstanceKey engine = {});
	Engine::Instance &createEngine(Engine::DataKey dataKey);
	Disc::Instance &createDisc(Disc::SetKey setKey);
	Team::Instance &createTeam(std::string_view name = "");

	extern void saveToFile(const std::string &path);
	extern void loadFromFile(const std::string &path);

	// extern Serialization::Good::IGOOD saveToGOOD();
	// extern void loadFromGOOD(const Serialization::Good::IGOOD &);
}// namespace Store
