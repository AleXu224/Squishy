#pragma once

#include "core/core.hpp"
#include "cstdint"
#include "weapon/instance.hpp"


namespace UI {
	using namespace squi;
	struct WeaponCard : StatefulWidget {
		enum class Actions : uint8_t {
			list,
			character,
			showcase,
		};

		// Args
		Key key;
		Args widget{};
		Weapon::Instance &weapon;
		Actions actions = WeaponCard::Actions::list;

		struct State : WidgetState<WeaponCard> {
			VoidObserver weaponUpdateObserver{};

			void initState() override {
				weaponUpdateObserver = widget->weapon.updateEvent.observe([this]() {
					setState([]() {});
				});
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI