#pragma once

#include "agent/key.hpp"
#include "core/core.hpp"


namespace UI {
	using namespace squi;
	struct AgentPage : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Agent::InstanceKey agentKey{};

		struct State : WidgetState<AgentPage> {
			Child build(const Element &element) override;
		};
	};
}// namespace UI