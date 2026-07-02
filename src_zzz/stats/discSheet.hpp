#pragma once

#include "sheet.hpp"
#include <optional>


namespace Stats {
	struct Loadout;
	struct DiscSheet {
		std::array<std::optional<Stats::SheetSimplified<float> *>, 6> equippedDiscs{};
	};
}// namespace Stats