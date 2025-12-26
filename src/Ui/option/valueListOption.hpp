#pragma once

#include "core/core.hpp"

#include "character/key.hpp"
#include "option/valueList.hpp"
#include "team/key.hpp"


namespace UI {
	using namespace squi;
	struct ValueListOption : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Option::ValueList &option;
		std::variant<Character::InstanceKey, Team::InstanceKey> instanceKey{};
		Formula::Context ctx;

		struct State : WidgetState<ValueListOption> {
			Child build(const Element &element) override;
		};
	};
}// namespace UI