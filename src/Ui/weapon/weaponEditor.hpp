#pragma once

#include "widget.hpp"

#include "weapon/instance.hpp"

namespace UI {
	struct WeaponEditor {
		// Args
		squi::Widget::Args widget{};
		Weapon::Instance weapon;
		std::function<void(Weapon::Instance)> onSubmit{};

		struct Storage {
			// Data
			Weapon::Instance weapon;
		};

		operator squi::Child() const;
	};
}// namespace UI