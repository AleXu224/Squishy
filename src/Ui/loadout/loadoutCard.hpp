#pragma once

#include "character/key.hpp"
#include "core/core.hpp"


namespace UI {
	using namespace squi;
	struct LoadoutCard : StatefulWidget {
		// Args
		Key key;
		Args widget{};
		Character::InstanceKey characterKey;
		std::optional<uint32_t> loadoutIndex = std::nullopt;

		struct State : WidgetState<LoadoutCard> {
			Child build(const Element &element) override;
		};
	};
}// namespace UI