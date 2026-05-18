#pragma once

#include "character/key.hpp"
#include "core/core.hpp"
#include "optimization/solutionUpgrade.hpp"


namespace UI {
	using namespace squi;
	struct OptimizationResultUpgrade : StatelessWidget {
		// Args
		Key key;
		Args widget{};
		Character::InstanceKey characterKey;
		::Optimization::SolutionUpgrade solution;
		uint32_t entryIndex;

		[[nodiscard]] Child build(const Element &) const;
	};
}// namespace UI