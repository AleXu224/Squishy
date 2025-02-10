#pragma once

#include "widget.hpp"

#include "character/instance.hpp"

namespace UI {
	struct CharacterEditor {
		// Args
		squi::Widget::Args widget{};
		Character::Instance character;
		std::function<void(Character::Instance)> onSubmit{};

		struct Storage {
			// Data
			Character::Instance character;
		};

		operator squi::Child() const;
	};
}// namespace UI