#pragma once

#include "character/key.hpp"
#include "core/core.hpp"


namespace UI {
	using namespace squi;
	struct CharacterPage : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Character::InstanceKey characterKey{};

		struct State : WidgetState<CharacterPage> {
			Child build(const Element &element) override;
		};
	};
}// namespace UI