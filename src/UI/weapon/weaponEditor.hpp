#pragma once

#include "core/core.hpp"
#include "weapon/defaultWeapons.hpp"
#include "weapon/instance.hpp"


namespace UI {
	using namespace squi;
	struct WeaponEditor : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Weapon::Instance weapon;
		std::function<void(Weapon::Instance)> onSubmit{};

		struct State : WidgetState<WeaponEditor> {
			// Weapon instance has no default constructor
			Weapon::Instance weapon = Weapon::Instance{
				Weapon::defaultWeapons.at(Misc::WeaponType::sword),
				{0},
			};
			VoidObservable closeEvent{};

			void initState() override {
				weapon = widget->weapon;
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI