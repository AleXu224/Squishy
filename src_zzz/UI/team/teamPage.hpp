#pragma once

#include "core/core.hpp"
#include "observer.hpp"
#include "store.hpp"

namespace UI {
	using namespace squi;
	struct TeamPage : StatefulWidget {
		// Args
		Key key;
		Args widget{};

		struct State : WidgetState<TeamPage> {
			VoidObserver teamListUpdateObserver;

			void initState() override {
				teamListUpdateObserver = ::Store::teamListUpdateEvent.observe([this]() {
					this->element->markNeedsRebuild();
				});
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI