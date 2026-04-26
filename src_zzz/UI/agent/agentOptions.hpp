#pragma once

#include "agent/key.hpp"
#include "core/core.hpp"


namespace UI {
	using namespace squi;
	struct AgentOptions : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Agent::InstanceKey agentKey{};

		struct State : WidgetState<AgentOptions> {
			Child build(const Element &element) override;
		};
	};
}// namespace UI