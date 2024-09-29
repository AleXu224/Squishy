#pragma once

#include "sheet.hpp"
#include <optional>


namespace Stats {
	struct Loadout;
	struct ArtifactSheet {
		std::array<std::optional<Stats::Sheet<float> *>, 5> equippedArtifacts{};
	};
}// namespace Stats