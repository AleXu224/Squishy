#pragma once

#include "widget.hpp"
#include "weapon/instance.hpp"

namespace UI {
	struct WeaponCard {
		// Args
		squi::Widget::Args widget{};
        Weapon::Instance &weapon;
		bool hasActions = true;

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI