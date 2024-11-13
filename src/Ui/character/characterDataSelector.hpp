#pragma once

#include "widget.hpp"

#include "character/key.hpp"
#include "misc/element.hpp"
#include "misc/weaponType.hpp"


namespace UI {
	struct CharacterDataSelector {
		// Args
		squi::Widget::Args widget{};
		std::function<void(Character::DataKey)> onSelect{};

		struct Storage {
			// Data
			std::unordered_map<Misc::Element, bool> characterElements{};
			std::unordered_map<Misc::WeaponType, bool> characterWeapon{};
		};

		operator squi::Child() const;
	};
}// namespace UI