#pragma once

#include "agent/key.hpp"
#include "core/core.hpp"
#include "enemy/key.hpp"
#include "observer.hpp"
#include "store.hpp"
#include "team/key.hpp"


namespace UI {
	using namespace squi;
	struct TCOptimization : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Agent::InstanceKey agentKey;
		std::optional<Team::InstanceKey> teamKey{};
		Enemy::Key enemyKey{};

		struct State : WidgetState<TCOptimization> {
			VoidObserver agentUpdateEvent;

			void initState() override {
				auto &agent = ::Store::agents.at(widget->agentKey);
				agentUpdateEvent = agent.updateEvent.observe([this]() {
					setState([]() {});
				});
			}

			Child build(const Element &element) override;
		};
	};
}// namespace UI