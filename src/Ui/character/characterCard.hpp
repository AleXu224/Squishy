#pragma once

#include "widget.hpp"
#include "character/instance.hpp"

namespace UI {
	struct CharacterCard {
		// Args
		squi::Widget::Args widget{};
        Character::Instance &character;

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI