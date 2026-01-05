#pragma once

#include "core/core.hpp"
#include "observer.hpp"
#include "store.hpp"

namespace UI {
	using namespace squi;
	struct CharacterList : StatefulWidget {
		// Args
		Key key;

		struct State : WidgetState<CharacterList> {
			VoidObserver characterListUpdateEvent;

			void initState() override {
				characterListUpdateEvent = ::Store::characterListUpdateEvent.observe([this]() {
					setState([&]() {});
				});
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI