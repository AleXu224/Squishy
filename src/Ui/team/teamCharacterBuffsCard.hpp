#pragma once

#include "widget.hpp"

#include "character/instance.hpp"
#include "team/instance.hpp"

namespace UI {
	struct TeamCharacterBuffsCard {
		// Args
		squi::Widget::Args widget{};
		Team::Instance &team;
		Character::Instance &character;

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI