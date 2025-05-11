#pragma once

#include "misc/reaction.hpp"
#include "observer.hpp"
#include "option.hpp"
#include "source.hpp"


namespace Combo {
	struct Entry {
		float multiplier = 1.f;
		Misc::NodeReaction reaction = Misc::NodeReaction::none;
		Combo::Source::Types source;
		squi::VoidObservable optionUpdateEvent{};
		std::vector<Combo::Option> options{};
	};
	struct StateChangeEntry {
		float multiplier = 1.f;
		squi::VoidObservable optionUpdateEvent{};
		std::vector<Combo::Option> options{};
	};

	using EntryTypes = std::variant<Entry, StateChangeEntry>;
}// namespace Combo