#pragma once

#include "entry.hpp"
#include "list"
#include "observer.hpp"


namespace Combo {
	struct Combo {
		InstanceKey instanceKey;
		std::string name;
		squi::VoidObservable updateEvent{};
		std::list<EntryTypes> entries{};

		[[nodiscard]] float eval(const Formula::Context &context) const;
	};
}// namespace Combo