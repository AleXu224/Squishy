#pragma once

#include "character/key.hpp"
#include "core/core.hpp"
#include "optimization/solution.hpp"


namespace UI {
	using namespace squi;
	struct OptimizationResult : StatelessWidget {
		// Args
		Key key;
		Args widget{};
		Character::InstanceKey characterKey;
		::Optimization::Solution solution;
		uint32_t entryIndex;

		[[nodiscard]] Child build(const Element &) const;
	};
}// namespace UI