#pragma once

#include "agent/key.hpp"
#include "core/core.hpp"


namespace UI {
	using namespace squi;
	struct AgentCardBanner : StatelessWidget {
		// Args
		Key key;
		Agent::InstanceKey agentKey;

		[[nodiscard]] Child build(const Element &) const;
	};
}// namespace UI
