#pragma once

#include "core/core.hpp"
#include "misc/specialty.hpp"


#include "agent/key.hpp"
#include "misc/attribute.hpp"


namespace UI {
	using namespace squi;
	struct AgentSelector : StatefulWidget {
		// Args
		Key key;
		std::function<void(Agent::InstanceKey)> onSelect{};

		struct State : WidgetState<AgentSelector> {
			std::unordered_map<Misc::Attribute, bool> agentAttributes{};
			std::unordered_map<Misc::Specialty, bool> agentSpecialty{};

			void initState() override;

			Child build(const Element &element) override;
		};
	};
}// namespace UI