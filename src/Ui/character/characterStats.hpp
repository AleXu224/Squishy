#pragma once

#include "character/key.hpp"
#include "formula/formulaContext.hpp"


#include "core/core.hpp"

namespace UI {
	using namespace squi;
	struct CharacterStats : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Formula::Context ctx;
		Character::InstanceKey characterKey;

		struct State : WidgetState<CharacterStats> {
			Child build(const Element &element) override;
		};
	};
}// namespace UI