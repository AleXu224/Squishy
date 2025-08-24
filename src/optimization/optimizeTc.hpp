#pragma once

#include "character/instance.hpp"
#include "formula/formulaContext.hpp"
#include "optimization/optionsTc.hpp"
#include "solutionTc.hpp"

namespace Optimization {
	struct TCOptimization {
		Character::Instance &character;
		Formula::Context ctx;
		Formula::FloatNode optimizedNode;

		const ::Optimization::OptionsTc &options;

		SolutionTC optimize() const;
	};
}// namespace Optimization