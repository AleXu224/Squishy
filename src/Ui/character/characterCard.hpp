#pragma once

#include "character/key.hpp"
#include "core/core.hpp"


namespace UI {
	using namespace squi;
	struct CharacterCard : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Character::InstanceKey characterKey{};

		struct State : WidgetState<CharacterCard> {
			Child build(const Element &element) override;
		};
	};
}// namespace UI