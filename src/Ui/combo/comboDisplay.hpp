#pragma once

#include "character/key.hpp"
#include "formula/formulaContext.hpp"
#include "widget.hpp"


namespace UI {
	struct ComboDisplay {
		// Args
		squi::Widget::Args widget{};
		Character::InstanceKey characterKey;
		Formula::Context ctx;

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI