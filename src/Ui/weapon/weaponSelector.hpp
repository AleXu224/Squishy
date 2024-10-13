#pragma once

#include "stats/weaponBase.hpp"
#include "weapon/key.hpp"
#include "widget.hpp"

namespace UI {
	struct WeaponSelector {
		// Args
		squi::Widget::Args widget{};
		std::optional<WeaponType> type{};
		std::function<void(Weapon::DataKey)> onSelect{};

		operator squi::Child() const;
	};
}// namespace UI