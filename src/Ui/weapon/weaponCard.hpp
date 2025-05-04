#pragma once

#include "cstdint"
#include "weapon/instance.hpp"
#include "widget.hpp"


namespace UI {
	struct WeaponCard {
		enum class Actions : uint8_t {
			list,
			character,
			showcase,
		};

		// Args
		squi::Widget::Args widget{};
		Weapon::Instance &weapon;
		Actions actions = WeaponCard::Actions::list;

		struct Storage {
			// Data
		};

		operator squi::Child() const;
	};
}// namespace UI