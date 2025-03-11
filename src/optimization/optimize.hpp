#pragma once

#include "character/instance.hpp"
#include "formula/formulaContext.hpp"

namespace Optimization {
	struct Optimization {
		Character::Instance &character;
		Formula::Context ctx;
		Formula::FloatNode optimizedNode;

		void optimize() const;
	};
}// namespace Optimization