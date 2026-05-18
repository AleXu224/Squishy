#pragma once

#include "agent/key.hpp"
#include "core/core.hpp"
#include "optimization/solutionUpgrade.hpp"


namespace UI {
	using namespace squi;
	struct OptimizationResultUpgrade : StatelessWidget {
		// Args
		Key key;
		Args widget{};
		Agent::InstanceKey agentKey;
		::Optimization::SolutionUpgrade solution;
		uint32_t entryIndex;

		[[nodiscard]] Child build(const Element &) const;
	};
}// namespace UI