#pragma once

#include "widget.hpp"

#include "character/key.hpp"
#include "conditional/boolean.hpp"


namespace UI {
	struct ToggleConditional {
		// Args
		squi::Widget::Args widget{};
		Conditional::Boolean &conditional;
		Character::Key characterKey;

		struct Storage {
			// Data
			bool active;
		};

		operator squi::Child() const;
	};
}// namespace UI