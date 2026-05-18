#pragma once

#include "agent/instance.hpp"
#include "formula/base.hpp"
#include "optimization/optionsUpgrade.hpp"
#include "solutionUpgrade.hpp"

namespace Optimization {
	struct UpgradeOptimization {
		Agent::Instance &agent;
		Formula::Context ctx;
		Formula::FloatNode optimizedNode;

		const ::Optimization::OptionsUpgrade &options;

		SolutionsUpgrade optimize() const;
	};
}// namespace Optimization