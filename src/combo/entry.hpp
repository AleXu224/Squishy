#pragma once

#include "misc/reaction.hpp"
#include "option.hpp"
#include "source.hpp"


namespace Combo {
	struct Entry {
		float multiplier = 1.f;
		Misc::NodeReaction reaction = Misc::NodeReaction::none;
		Combo::Source::Types source;
		std::vector<Combo::Option> options{};
	};
}// namespace Combo