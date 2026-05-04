#pragma once

#include "agent/instance.hpp"
#include "core/core.hpp"

namespace UI {
	using namespace squi;
	struct TCOptimizationSettings : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Agent::Instance &agent;
		Formula::Context ctx;

		struct State : WidgetState<TCOptimizationSettings> {
			VoidObserver agentUpdateEvent;

			void initState() override {
				agentUpdateEvent = widget->agent.updateEvent.observe([this]() {
					setState([&]() {});
				});
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI