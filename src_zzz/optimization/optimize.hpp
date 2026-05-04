#pragma once

#include "agent/instance.hpp"
#include "formula/base.hpp"
#include "optimization/options.hpp"
#include "solution.hpp"
#include "team/instance.hpp"


namespace Optimization {
	struct OptimizationThreadData {
		Agent::Instance agent;
		std::array<std::optional<Agent::Instance>, 4> teamAgents;
		Team::Instance team;
		Formula::Context ctx;

		OptimizationThreadData(Agent::Instance &agent, const Formula::Context &ctx);
	};

	struct Optimization {
		Agent::Instance &agent;
		Formula::Context ctx;
		Formula::FloatNode optimizedNode;

		const ::Optimization::Options &options;

		Solutions optimize() const;
	};
}// namespace Optimization