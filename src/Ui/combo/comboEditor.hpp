#pragma once

#include "combo/combo.hpp"
#include "widget.hpp"


namespace UI {
	struct ComboEditor {
		// Args
		squi::Widget::Args widget{};
		Combo::Combo &combo;
		Character::InstanceKey characterKey;
		Formula::Context ctx;
		std::function<void(Combo::Combo)> onSave;

		struct Storage {
			// Data
			Combo::Combo combo;
		};

		operator squi::Child() const;
	};
}// namespace UI