#pragma once

#include "widget.hpp"

#include "character/key.hpp"

namespace UI {
	struct CharacterLoadouts {
		// Args
		squi::Widget::Args widget{};
		Character::InstanceKey characterKey;

		operator squi::Child() const;
	};
}// namespace UI