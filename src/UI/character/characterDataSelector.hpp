#pragma once

#include "core/core.hpp"

#include "character/key.hpp"
#include "misc/element.hpp"
#include "misc/weaponType.hpp"
#include "observer.hpp"


namespace UI {
	using namespace squi;
	struct CharacterDataSelector : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		std::function<void(Character::DataKey)> onSelect{};

		struct State : WidgetState<CharacterDataSelector> {
			VoidObservable closeEvent{};
			std::unordered_map<Misc::Element, bool> characterElements{};
			std::unordered_map<Misc::WeaponType, bool> characterWeapon{};

			void initState() override {
				for (const auto &element: Misc::characterElements) {
					characterElements[element] = true;
				}
				for (const auto &weaponType: Misc::weaponTypes) {
					characterWeapon[weaponType] = true;
				}
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI