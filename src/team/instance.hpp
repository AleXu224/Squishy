#pragma once

#include "key.hpp"
#include "observer.hpp"
#include "stats/team.hpp"
#include "string_view"


namespace Team {
	struct Instance {
		Team::Key key;
		std::string_view name;
		squi::VoidObservable updateEvent{};

		Stats::Team stats{};
	};
}// namespace Team