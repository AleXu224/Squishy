#pragma once

#include "widget.hpp"
#include "navigator.hpp"
#include "character/key.hpp"

namespace UI {
	struct CharacterPage {
		// Args
		squi::Widget::Args widget{};
		Character::Key characterKey;
		squi::Navigator::Controller controller;

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI