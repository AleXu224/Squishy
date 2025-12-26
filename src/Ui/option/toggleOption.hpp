#pragma once

#include "core/core.hpp"

#include "character/key.hpp"
#include "option/boolean.hpp"
#include "team/key.hpp"


namespace UI {
	using namespace squi;
	struct ToggleOption : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Option::Boolean &option;
		std::variant<Character::InstanceKey, Team::InstanceKey> instanceKey{};
		Formula::Context ctx;

		struct State : WidgetState<ToggleOption> {
			Child build(const Element &element) override;
		};
	};
}// namespace UI