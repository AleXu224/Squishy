#pragma once

#include "core/core.hpp"
#include "store.hpp"

namespace UI {
	using namespace squi;
	struct WeaponPage : StatefulWidget {
		// Args
		Key key;

		struct State : WidgetState<WeaponPage> {
			VoidObserver weaponListUpdateObserver;

			void initState() override {
				weaponListUpdateObserver = ::Store::weaponListUpdateEvent.observe([this]() {
					this->element->markNeedsRebuild();
				});
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI