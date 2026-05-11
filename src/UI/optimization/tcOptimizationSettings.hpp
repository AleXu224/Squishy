#pragma once

#include "character/instance.hpp"
#include "core/core.hpp"

namespace UI {
	using namespace squi;
	struct TCOptimizationSettings : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Character::Instance &character;
		Formula::Context ctx;

		struct State : WidgetState<TCOptimizationSettings> {
			VoidObserver characterUpdateEvent;

			void initState() override {
				characterUpdateEvent = widget->character.updateEvent.observe([this]() {
					setState([&]() {});
				});
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI