#pragma once

#include "core/core.hpp"
#include "misc/weaponType.hpp"
#include "observer.hpp"
#include "weapon/key.hpp"


namespace UI {
	using namespace squi;
	struct WeaponSelector : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		std::optional<Misc::WeaponType> type{};
		std::function<void(Weapon::DataKey)> onSelect{};

		struct State : WidgetState<WeaponSelector> {
			VoidObservable closeEvent{};
			std::unordered_map<Misc::WeaponType, bool> weaponTypes{};

			Child build(const Element &element) override;
		};
	};
}// namespace UI