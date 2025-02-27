#pragma once

#include "entry.hpp"
#include "list"
#include "observer.hpp"



namespace Combo {
	struct Combo {
		std::string name;
		squi::VoidObservable updateEvent{};
		std::list<Entry> entries{};
	};
}// namespace Combo