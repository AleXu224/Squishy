#pragma once

#include "agent/key.hpp"
#include "enemy/key.hpp"
#include "observer.hpp"
#include "store.hpp"
#include "team/key.hpp"


#include "core/core.hpp"

namespace UI {
	using namespace squi;
	struct AgentDetails : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Agent::InstanceKey agentKey{};
		std::optional<Team::InstanceKey> teamKey{};
		Enemy::Key enemyKey{};

		struct State : WidgetState<AgentDetails> {
			VoidObserver agentUpdateEvent;
			VoidObserver loadoutUpdateEvent;

			void initState() override {
				agentUpdateEvent = ::Store::agents.at(widget->agentKey).updateEvent.observe([this]() {
					this->setState();
				});
				loadoutUpdateEvent = ::Store::agents.at(widget->agentKey).loadoutChangedEvent.observe([this]() {
					this->setState();
				});
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI