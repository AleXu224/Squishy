#pragma once

#include "key.hpp"
#include "observer.hpp"
#include "stats/team.hpp"
#include "string"


namespace Team {
	struct Instance {
		Team::InstanceKey instanceKey;
		std::string name{};
		squi::VoidObservable updateEvent{};

		Stats::Team stats{};
	};
}// namespace Team