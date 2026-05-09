#pragma once

#include "stats/stat.hpp"
#include "core/core.hpp"

namespace UI {
	using namespace squi;
	struct StatDisplay : StatelessWidget {
		// Args
		Key key;
		bool isTransparent;
		bool showRolls = false;
		uint32_t rarity = 5;
		StatValue stat;

		[[nodiscard]] Child build(const Element &) const;
	};

}// namespace UI