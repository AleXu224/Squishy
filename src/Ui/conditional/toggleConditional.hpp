#pragma once

#include "widget.hpp"

#include "conditional/boolean.hpp"

namespace UI {
	struct ToggleConditional {
		// Args
		squi::Widget::Args widget{};
        Conditional::Boolean &conditional;

		struct Storage {
			// Data
            bool active;
		};

		operator squi::Child() const;
	};
}// namespace UI