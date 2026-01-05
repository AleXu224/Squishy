#pragma once

#include "core/core.hpp"
#include "misc/weaponType.hpp"


#include "character/key.hpp"
#include "misc/element.hpp"


namespace UI {
	using namespace squi;
	struct CharacterSelector : StatefulWidget {
		// Args
		Key key;
		std::function<void(Character::InstanceKey)> onSelect{};

		struct State : WidgetState<CharacterSelector> {
			std::unordered_map<Misc::Element, bool> characterElements{};
			std::unordered_map<Misc::WeaponType, bool> characterWeapon{};

			void initState() override;

			Child build(const Element &element) override;
		};
	};
}// namespace UI