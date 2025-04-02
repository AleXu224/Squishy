#pragma once

#include "combo/combo.hpp"
#include "option/option.hpp"
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
			std::vector<std::unique_ptr<Option::Types>> optsCopy;
		};

		operator squi::Child() const;
	};
}// namespace UI