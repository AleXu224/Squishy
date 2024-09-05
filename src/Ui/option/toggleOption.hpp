#pragma once

#include "widget.hpp"

#include "character/key.hpp"
#include "option/boolean.hpp"


namespace UI {
	struct ToggleOption {
		// Args
		squi::Widget::Args widget{};
		Option::Boolean &option;
		Character::Key characterKey{};

		struct Storage {
			// Data
			bool active;
		};

		operator squi::Child() const;
	};
}// namespace UI