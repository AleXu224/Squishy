#pragma once

#include "source.hpp"


namespace Combo {
	struct Entry {
		float multiplier = 1.f;
		Misc::NodeReaction reaction = Misc::NodeReaction::none;
		Combo::Source::Types source;
	};
}// namespace Combo