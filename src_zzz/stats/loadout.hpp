#pragma once

#include "agent/key.hpp"
#include "engine/key.hpp"
#include "stats/agent.hpp"
#include "stats/disc.hpp"
#include "stats/engine.hpp"

#include <memory>


namespace Stats {
	struct Loadout {
		::Engine::InstanceKey engineInstanceKey;
		Stats::Engine *engine;
		Stats::Disc disc;

		void init(Option::TypesMap &options);

		void swapEngine(::Engine::InstanceKey);
	};

	struct State {
		::Agent::InstanceKey instanceKey;
		Stats::Agent stats;
		std::optional<uint32_t> loadoutIndex = std::nullopt;
		std::shared_ptr<Option::TypesMap> options = std::make_shared<Option::TypesMap>();
		Loadout equippedLoadout;
		std::vector<Loadout> loadouts{};

		void init();

		[[nodiscard]] inline Loadout &loadoutByIndex(const std::optional<uint32_t> &index) {
			if (!index.has_value()) return equippedLoadout;
			return loadouts.at(index.value());
		}
		[[nodiscard]] inline const Loadout &loadoutByIndex(const std::optional<uint32_t> &index) const {
			if (!index.has_value()) return equippedLoadout;
			return loadouts.at(index.value());
		}
		[[nodiscard]] inline Loadout &loadout() {
			return loadoutByIndex(loadoutIndex);
		}
		[[nodiscard]] inline const Loadout &loadout() const {
			return loadoutByIndex(loadoutIndex);
		}
	};
}// namespace Stats