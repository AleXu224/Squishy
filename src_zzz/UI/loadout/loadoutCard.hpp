#pragma once

#include "agent/key.hpp"
#include "core/core.hpp"


namespace UI {
	using namespace squi;
	struct LoadoutCard : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Agent::InstanceKey agentKey;
		std::optional<uint32_t> loadoutIndex = std::nullopt;

		struct State : WidgetState<LoadoutCard> {
			Child build(const Element &element) override;
		};
	};
}// namespace UI