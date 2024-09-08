#pragma once

#include "character/key.hpp"
#include "navigator.hpp"
#include "widget.hpp"


namespace UI {
	struct CharacterPage {
		// Args
		squi::Widget::Args widget{};
		Character::InstanceKey characterKey{};
		squi::Navigator::Controller controller;

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI