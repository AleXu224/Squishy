#pragma once

#include "core/core.hpp"

#include "character/key.hpp"
#include "observer.hpp"
#include "store.hpp"

namespace UI {
	using namespace squi;
	struct CharacterLoadouts : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Character::InstanceKey characterKey;

		struct State : WidgetState<CharacterLoadouts> {
			VoidObservable closeEvent{};
			VoidObserver characterUpdateEvent;

			void initState() override {
				auto &character = ::Store::characters.at(widget->characterKey);
				characterUpdateEvent = character.updateEvent.observe([this]() {
					setState([&]() {});
				});
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI