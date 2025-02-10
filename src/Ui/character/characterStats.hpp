#pragma once

#include "character/key.hpp"
#include "formula/formulaContext.hpp"


#include "widget.hpp"

namespace UI {
	struct CharacterStats {
		// Args
		squi::Widget::Args widget{};
		Formula::Context ctx;
		Character::InstanceKey characterKey;

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI