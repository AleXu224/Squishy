#pragma once

#include "source.hpp"

namespace Combo {
	struct Entry {
		float multiplier = 1.f;
		Misc::Reaction reaction;
		Combo::Source::Types source;
	};
}// namespace Combo