#pragma once

#include "misc/weaponType.hpp"
#include "widget.hpp"

#include "character/key.hpp"
#include "misc/element.hpp"


namespace UI {
	struct CharacterSelector {
		// Args
		squi::Widget::Args widget{};
		std::function<void(Character::InstanceKey)> onSelect{};

		struct Storage {
			// Data
			std::unordered_map<Misc::Element, bool> characterElements{};
			std::unordered_map<Misc::WeaponType, bool> characterWeapon{};
		};

		operator squi::Child() const;
	};
}// namespace UI