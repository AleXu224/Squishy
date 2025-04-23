#pragma once

#include "character/instance.hpp"
#include "formula/formulaContext.hpp"
#include "solution.hpp"

namespace Optimization {
	struct Optimization {
		Character::Instance &character;
		Formula::Context ctx;
		Formula::FloatNode optimizedNode;

		const ::Optimization::Options &options;

		Solutions optimize() const;
	};
}// namespace Optimization