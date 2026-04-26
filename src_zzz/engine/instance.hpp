#pragma once

#include "agent/key.hpp"
#include "engine/key.hpp"
#include "observer.hpp"
#include "stats/engine.hpp"


namespace Engine {
	struct Instance {
		InstanceKey instanceKey;
		Stats::Engine stats;

		squi::VoidObservable updateEvent{};

		Instance(const Engine::DataKey &dataKey, const Engine::InstanceKey &instanceKey);
		void equipOn(Agent::InstanceKey instanceKey, std::optional<uint32_t> loadout = std::nullopt) const;
		[[nodiscard]] std::vector<Agent::InstanceKey> equippedOn() const;
		[[nodiscard]] bool isEquippedOn(Agent::InstanceKey instanceKey) const;
		[[nodiscard]] std::vector<Agent::InstanceKey> usedOn() const;
		[[nodiscard]] bool isUsedOn(Agent::InstanceKey instanceKey) const;
	};
}// namespace Engine
