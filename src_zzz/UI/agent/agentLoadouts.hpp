#pragma once

#include "core/core.hpp"

#include "agent/key.hpp"
#include "observer.hpp"
#include "store.hpp"

namespace UI {
	using namespace squi;
	struct AgentLoadouts : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Agent::InstanceKey agentKey;

		struct State : WidgetState<AgentLoadouts> {
			VoidObservable closeEvent{};
			VoidObserver agentUpdateEvent;

			void initState() override {
				auto &agent = ::Store::agents.at(widget->agentKey);
				agentUpdateEvent = agent.updateEvent.observe([this]() {
					setState([&]() {});
				});
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI