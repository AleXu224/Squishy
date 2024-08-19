#pragma once

#include "sheet.hpp"


namespace Stats {
	struct Loadout;
	struct ArtifactSheet {
		StatSheet<7, 2, 0, 0> stats{};
	};
}// namespace Stats