#pragma once

#include "character/key.hpp"
#include "core/core.hpp"


namespace UI {
	using namespace squi;
	struct CharacterCardBanner : StatelessWidget {
		// Args
		Key key;
		Character::InstanceKey characterKey;

		[[nodiscard]] Child build(const Element &) const;
	};
}// namespace UI
