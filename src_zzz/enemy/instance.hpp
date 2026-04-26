#pragma once

#include "key.hpp"
#include "observer.hpp"
#include "stats/enemy.hpp"


namespace Enemy {
	struct Instance {
		Key key{};
		std::string_view name;
		squi::VoidObservable updateEvent{};

		Stats::Enemy stats{};
	};
}// namespace Enemy