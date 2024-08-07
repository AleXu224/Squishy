#pragma once

#include "navigator.hpp"
#include "widget.hpp"

#include "character/key.hpp"


namespace UI {
	struct CharacterStatInspector {
		// Args
		Character::Key characterKey;

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI