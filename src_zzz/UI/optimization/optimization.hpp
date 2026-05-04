#pragma once

#include "agent/key.hpp"
#include "core/core.hpp"
#include "enemy/key.hpp"
#include "optimization/solution.hpp"
#include "team/key.hpp"


namespace UI {
	using namespace squi;
	struct Optimization : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Agent::InstanceKey agentKey;
		std::optional<Team::InstanceKey> teamKey{};
		Enemy::Key enemyKey{};

		struct State : WidgetState<Optimization> {
			::Optimization::Solutions solutions;

			Child build(const Element &element) override;
		};
	};
}// namespace UI