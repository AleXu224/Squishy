#pragma once

#include "agent/key.hpp"
#include "core/core.hpp"
#include "formula/base.hpp"


namespace UI {
	using namespace squi;
	struct ComboDisplay : StatelessWidget {
		// Args
		Key key;
		Args widget{};
		Agent::InstanceKey agentKey;
		Formula::Context ctx;

		[[nodiscard]] Child build(const Element &) const;
	};
}// namespace UI