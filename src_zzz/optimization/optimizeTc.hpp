#pragma once

#include "agent/instance.hpp"
#include "formula/base.hpp"
#include "optimization/optionsTc.hpp"
#include "solutionTc.hpp"

namespace Optimization {
	struct TCOptimization {
		Agent::Instance &agent;
		Formula::Context ctx;
		Formula::FloatNode optimizedNode;

		const ::Optimization::OptionsTc &options;

		SolutionTC optimize() const;
	};
}// namespace Optimization