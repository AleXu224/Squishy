#pragma once

#include "core/core.hpp"

#include "agent/instance.hpp"
#include "team/instance.hpp"

namespace UI {
	using namespace squi;
	struct TeamAgentBuffsCard : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Team::Instance &team;
		Agent::Instance &agent;

		struct State : WidgetState<TeamAgentBuffsCard> {
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