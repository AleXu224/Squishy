#pragma once

#include "agent/key.hpp"
#include "combo/key.hpp"
#include "combo/override.hpp"
#include "disc/key.hpp"
#include "disc/slot.hpp"
#include "engine/key.hpp"
#include "misc/anomaly.hpp"
#include "node/entry.hpp"
#include "node/node.hpp"
#include "variant"


namespace Combo::Source {
	struct Agent {
		::Agent::DataKey key;
		Node::AgentSlot slot;
		size_t index;

		[[nodiscard]] Node::Instance resolve(const Overrides &overrides) const;
	};

	struct Combo {
		::Agent::InstanceKey agentKey;
		::Combo::InstanceKey comboKey;

		[[nodiscard]] Node::Instance resolve(const Overrides &options) const;
	};

	struct Engine {
		::Engine::DataKey key;
		size_t index;

		[[nodiscard]] Node::Instance resolve(const Overrides &options) const;
	};

	struct Disc {
		::Disc::SetKey key;
		::Disc::SetSlot slot;
		size_t index;

		[[nodiscard]] Node::Instance resolve(const Overrides &options) const;
	};

	struct Anomaly {
		Misc::Anomaly reaction;

		[[nodiscard]] Node::Instance resolve(const Overrides &options) const;
	};

	using Types = std::variant<Agent, Combo, Engine, Disc, Anomaly>;
}// namespace Combo::Source