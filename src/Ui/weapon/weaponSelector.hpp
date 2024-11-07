#pragma once

#include "misc/weaponType.hpp"
#include "weapon/key.hpp"
#include "widget.hpp"

namespace UI {
	struct WeaponSelector {
		// Args
		squi::Widget::Args widget{};
		std::optional<Misc::WeaponType> type{};
		std::function<void(Weapon::DataKey)> onSelect{};

		struct Storage{
			std::unordered_map<Misc::WeaponType, bool> weaponTypes{};
		};

		operator squi::Child() const;
	};
}// namespace UI