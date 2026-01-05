#pragma once

#include "core/core.hpp"

#include "character/instance.hpp"
#include "team/instance.hpp"

namespace UI {
	using namespace squi;
	struct TeamCharacterBuffsCard : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Team::Instance &team;
		Character::Instance &character;

		struct State : WidgetState<TeamCharacterBuffsCard> {
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