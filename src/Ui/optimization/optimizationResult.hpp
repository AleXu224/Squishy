#pragma once

#include "character/key.hpp"
#include "optimization/solution.hpp"
#include "widget.hpp"


namespace UI {
	struct OptimizationResult {
		// Args
		squi::Widget::Args widget{};
		Character::InstanceKey characterKey;
		::Optimization::Solution solution;
		uint32_t entryIndex;

		operator squi::Child() const;
	};
}// namespace UI