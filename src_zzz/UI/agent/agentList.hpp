#pragma once

#include "core/core.hpp"
#include "observer.hpp"
#include "store.hpp"

namespace UI {
	using namespace squi;
	struct AgentList : StatefulWidget {
		// Args
		Key key;

		struct State : WidgetState<AgentList> {
			VoidObserver agentListUpdateEvent;

			void initState() override {
				agentListUpdateEvent = ::Store::agentListUpdateEvent.observe([this]() {
					setState([&]() {});
				});
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI