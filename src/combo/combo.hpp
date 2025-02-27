#pragma once

#include "entry.hpp"
#include "list"
#include "observer.hpp"



namespace Combo {
	struct Combo {
		std::string name;
		squi::VoidObservable updateEvent{};
		std::list<Entry> entries{};

		[[nodiscard]] float eval(const Formula::Context &context) const;
	};
}// namespace Combo