#pragma once

#include "agent/key.hpp"
#include "formula/base.hpp"


#include "core/core.hpp"

namespace UI {
	using namespace squi;
	struct AgentStats : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Formula::Context ctx;
		Agent::InstanceKey agentKey;

		struct State : WidgetState<AgentStats> {
			Child build(const Element &element) override;
		};
	};
}// namespace UI