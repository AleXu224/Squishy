#pragma once

#include "character/key.hpp"
#include "widgetArgs.hpp"


namespace UI {
	struct CharacterCardBanner {
		// Args
		Character::InstanceKey characterKey;

		operator squi::Child() const;
	};
}// namespace UI
