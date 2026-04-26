#pragma once

#include "core/core.hpp"

#include "agent/key.hpp"
#include "misc/attribute.hpp"
#include "misc/specialty.hpp"
#include "observer.hpp"


namespace UI {
	using namespace squi;
	struct AgentDataSelector : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		std::function<void(Agent::DataKey)> onSelect{};

		struct State : WidgetState<AgentDataSelector> {
			VoidObservable closeEvent{};
			std::unordered_map<Misc::Attribute, bool> agentAttributes{};
			std::unordered_map<Misc::Specialty, bool> agentSpecialties{};

			void initState() override {
				for (const auto &attribute: Misc::agentAttributes) {
					agentAttributes[attribute] = true;
				}
				for (const auto &specialty: Misc::specialties) {
					agentSpecialties[specialty] = true;
				}
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI