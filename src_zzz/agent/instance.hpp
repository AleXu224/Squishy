#pragma once

#include "observer.hpp"

#include "agent/key.hpp"
#include "combo/combo.hpp"
// #include "optimization/options.hpp"
#include "stats/loadout.hpp"


namespace Agent {
	struct Instance {
		const InstanceKey instanceKey;
		const DataKey dataKey;
		Stats::State state;
		std::unordered_map<Combo::InstanceKey, Combo::Combo> combos;
		squi::VoidObservable updateEvent{};
		squi::VoidObservable loadoutChangedEvent{};
		// std::shared_ptr<Optimization::Options> optimizationOptions = std::make_shared<Optimization::Options>();

		explicit Instance(const InstanceKey &instanceKey, const DataKey &dataKey);
	};
}// namespace Agent
