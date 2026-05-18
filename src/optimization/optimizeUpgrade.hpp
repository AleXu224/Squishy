#pragma once

#include "character/instance.hpp"
#include "formula/base.hpp"
#include "optimization/optionsUpgrade.hpp"
#include "solutionUpgrade.hpp"

namespace Optimization {
	struct UpgradeOptimization {
		Character::Instance &character;
		Formula::Context ctx;
		Formula::FloatNode optimizedNode;

		const ::Optimization::OptionsUpgrade &options;

		SolutionsUpgrade optimize() const;
	};
}// namespace Optimization