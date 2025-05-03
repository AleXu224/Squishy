#pragma once

#include "observer.hpp"

#include "character/key.hpp"
#include "combo/combo.hpp"
#include "optimization/options.hpp"
#include "stats/loadout.hpp"


namespace Character {
	struct Instance {
		const InstanceKey instanceKey;
		const DataKey dataKey;
		Stats::State state;
		std::unordered_map<Combo::InstanceKey, Combo::Combo> combos;
		squi::VoidObservable updateEvent{};
		std::shared_ptr<Optimization::Options> optimizationOptions = std::make_shared<Optimization::Options>();

		explicit Instance(const InstanceKey &instanceKey, const DataKey &dataKey);
	};
}// namespace Character
