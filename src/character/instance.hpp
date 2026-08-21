#pragma once

#include "observer.hpp"

#include "character/key.hpp"
#include "combo/combo.hpp"
#include "optimization/options.hpp"
#include "stats/loadout.hpp"

#include <memory>


namespace Character {
	struct Instance {
		const InstanceKey instanceKey;
		const DataKey dataKey;
		Stats::State state;
		std::shared_ptr<std::map<Combo::InstanceKey, Combo::Combo>> combos = std::make_shared<std::map<Combo::InstanceKey, Combo::Combo>>();
		squi::VoidObservable updateEvent{};
		squi::VoidObservable loadoutChangedEvent{};
		std::shared_ptr<Optimization::Options> optimizationOptions = std::make_shared<Optimization::Options>();

		explicit Instance(const InstanceKey &instanceKey, const DataKey &dataKey);
	};
}// namespace Character
