#pragma once

#include "core/core.hpp"

#include "character/instance.hpp"
#include "team/instance.hpp"

namespace UI {
	using namespace squi;
	struct TeamCharacterBuffsCard : StatelessWidget {
		// Args
		Key key;
		Args widget{};
		Team::Instance &team;
		Character::Instance &character;

		[[nodiscard]] Child build(const Element &) const;
	};
}// namespace UI