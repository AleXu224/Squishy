#pragma once

#include "core/core.hpp"

#include "agent/instance.hpp"

namespace UI {
	using namespace squi;
	struct AgentEditor : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Agent::Instance agent;
		std::function<void(Agent::Instance)> onSubmit{};

		struct State : WidgetState<AgentEditor> {
			VoidObservable closeEvent{};
			std::optional<Agent::Instance> agent;

			void initState() override {
				agent.emplace(widget->agent);
			}

			void clampPromotion();
			void clampSkills();

			Child build(const Element &element) override;
		};
	};
}// namespace UI