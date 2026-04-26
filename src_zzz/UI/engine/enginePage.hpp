#pragma once

#include "core/core.hpp"
#include "store.hpp"

namespace UI {
	using namespace squi;
	struct EnginePage : StatefulWidget {
		// Args
		Key key;

		struct State : WidgetState<EnginePage> {
			VoidObserver engineListUpdateObserver;

			void initState() override {
				engineListUpdateObserver = ::Store::engineListUpdateEvent.observe([this]() {
					this->element->markNeedsRebuild();
				});
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI