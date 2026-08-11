#pragma once

#include "core/core.hpp"
#include "stats/stat.hpp"


namespace UI {
	using namespace squi;
	struct StatDisplay : StatelessWidget {
		// Args
		Key key;
		bool isTransparent;
		bool showRolls = false;
		uint32_t rarity = 5;
		std::variant<StatValue, DiscSubstat> stat;

		[[nodiscard]] Child build(const Element &) const;
	};

}// namespace UI