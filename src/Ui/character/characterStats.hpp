#pragma once

#include "character/key.hpp"

#include "widget.hpp"

namespace UI {
	struct CharacterStats {
		// Args
		squi::Widget::Args widget{};
		Character::Key characterKey;

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI